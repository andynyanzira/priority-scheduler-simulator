#!/usr/bin/env bash
set -e

npm install
g++ scheduler.cpp -o scheduler
chmod +x scheduler