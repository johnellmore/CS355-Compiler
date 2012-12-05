#!/bin/sh
rm -r coverage/*
lcov --directory . --capture --output-file coverage/coverage.info
genhtml --output-directory coverage coverage/coverage.info
