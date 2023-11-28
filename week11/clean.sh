#!/bin/bash
sudo umount ./lofsdisk
sudo losetup -d ${LOOP_DEVICE}
sudo rm -rf ./lofsdisk lofs.img ex1.out
