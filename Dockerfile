# Use a base image with necessary development tools
FROM ubuntu:latest

LABEL maintainer="Your Name <your.email@example.com>"

# Install required packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    nano \
    curl \
    gcc \
    clang \
    llvm \
    bpfcc-tools \
    libbpfcc-dev \
    libelf-dev \
    linux-headers-generic \
    # linux-headers-5.15.0-25-generic \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /usr/src/app

# Copy your source code into the container
COPY . .

# Set up entry point
CMD ["bash"]
