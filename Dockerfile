# Ubuntu 16.04 base image
FROM ubuntu:16.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and basic utilities
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    cmake \
 && rm -rf /var/lib/apt/lists/*

# Create a non-root user and working directory
RUN useradd -m builder \
 && mkdir -p /workspace \
 && chown builder:builder /workspace

USER builder
WORKDIR /workspace

# Default command (interactive)
CMD [ "bash" ]