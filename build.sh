#!/bin/bash

cat sample8 view.ts > combined.ts
tsc -t ES2015 combined.ts

