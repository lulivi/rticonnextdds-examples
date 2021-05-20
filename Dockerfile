# (c) 2021 Copyright, Real-Time Innovations, Inc.  All rights reserved.
# No duplications, whole or partial, manual or electronic, may be made
# without express written permission.  Any such copies, or revisions thereof,
# must display this notice unaltered.
# This code contains trade secrets of Real-Time Innovations, Inc.
FROM python:3.8-alpine3.13

RUN apk add gcc g++ clang openssl make cmake

USER jenkins

WORKDIR /home/jenkins
