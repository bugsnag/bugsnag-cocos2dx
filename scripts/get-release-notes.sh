#!/usr/bin/env bash
# Grabs the content under the first "##" line and prints it
awk 'start && /^## /{exit;};/^## /{start=1;next};start' CHANGELOG.md
