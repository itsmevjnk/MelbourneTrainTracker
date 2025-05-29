#!/bin/bash
docker buildx build --platform linux/amd64,linux/arm64 -t itsmevjnk/mtt_backend --push .