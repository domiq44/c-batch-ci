#!/bin/bash
set -euo pipefail

IMAGE_NAME="c-batch-ci"
IMAGE_TAG="latest"
TAR_FILE="${IMAGE_NAME}.tar"
JOB_NAME="c-batch-ci-job"

JOB_TEMPLATE="k8s/job.yaml.template"
JOB_FILE="k8s/job.yaml"

# Répertoire du projet
PROJECT_HOME="$HOME/dev/c-batch-ci"

# Timeouts configurables
POD_CREATION_TIMEOUT=60
JOB_COMPLETION_TIMEOUT=30

log() {
  echo "[$(date '+%H:%M:%S')] $1"
}

check_dependencies() {
  for cmd in docker kubectl k0s envsubst; do
    if ! command -v $cmd >/dev/null 2>&1; then
      echo "❌ Erreur : '$cmd' n'est pas installé."
      exit 1
    fi
  done
}

generate_manifest() {
  log "📄 Génération du manifest Kubernetes..."
  PROJECT_HOME="$PROJECT_HOME" envsubst < "$JOB_TEMPLATE" > "$JOB_FILE"
}

build_image() {
  log "🔨 Build Docker image..."
  docker build --pull --no-cache -t ${IMAGE_NAME}:${IMAGE_TAG} .
}

extract_binary() {
  log "📥 Extract binary from image..."
  local cid
  cid=$(docker create ${IMAGE_NAME}:${IMAGE_TAG})
  docker cp "$cid":/app/c-batch-ci ./c-batch-ci
  docker rm -f "$cid"
  chmod +x ./c-batch-ci
}

export_image() {
  log "📦 Export image to tar..."
  docker save ${IMAGE_NAME}:${IMAGE_TAG} -o ${TAR_FILE}
}

import_image() {
  log "📥 Import image into k0s containerd..."
  sudo k0s ctr images import ${TAR_FILE}
  rm -f ${TAR_FILE}
  log "🧹 Pruning old containerd images..."
  sudo k0s ctr images prune || true
}

deploy_job() {
  log "♻️ Redeploy Kubernetes Job..."
  kubectl delete job ${JOB_NAME} --ignore-not-found
  log "📄 Applying job manifest..."
  kubectl apply -f ${JOB_FILE}
}

wait_for_pod() {
  log "⏳ Waiting for pod to be created..."
  local start=$(date +%s)
  POD=""

  while [ -z "$POD" ]; do
    POD=$(kubectl get pods -l job-name=${JOB_NAME} -o jsonpath='{.items[0].metadata.name}' 2>/dev/null || true)
    if (( $(date +%s) - start > POD_CREATION_TIMEOUT )); then
      echo "❌ Timeout: Pod non créé après ${POD_CREATION_TIMEOUT}s"
      exit 1
    fi
    sleep 1
  done

  log "📦 Pod detected: $POD"
}

wait_for_completion() {
  log "⏳ Waiting for job to complete..."
  kubectl wait --for=condition=complete job/${JOB_NAME} --timeout=${JOB_COMPLETION_TIMEOUT}s
}

show_logs() {
  log "📄 Logs from $POD:"
  kubectl logs $POD --tail=200
}

cleanup() {
  log "🧹 Cleaning completed pod..."
  kubectl delete pod $POD --ignore-not-found
}

### MAIN PIPELINE ###
check_dependencies
generate_manifest
build_image
extract_binary
export_image
import_image
deploy_job
wait_for_pod
wait_for_completion
show_logs
cleanup

log "✅ Deployment finished"
