#!/bin/sh

~/hyper/bin/driver $2 -q `for i in $(seq $1); do echo $3; done` $4
