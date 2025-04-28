#!/bin/bash

OS_NAME=zjx_sgx
OS_IMAGE=/home/zjx/vm/ubuntu-24.04.2-live-server-amd64.iso
DISK_IMAGE=/home/zjx/vm/zjx_sgx.qcow2
DISK_SIZE=70

virt-install \
    --name $OS_NAME \
    --memory 8192 \
    --vcpus=4 \
    --cpu host \
    --disk path=$DISK_IMAGE,size=$DISK_SIZE \
    --location=$OS_IMAGE \
    --network default \
    --graphics none \
    --console pty,target_type=serial \
    --extra-args 'console=ttyS0,115200n8' \
    --accelerate \
    --hvm &
