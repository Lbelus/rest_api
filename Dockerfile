FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    clang \
    cmake \
    git \
    wget \
    unzip \
    pkg-config \
    libasio-dev \
    libsqlite3-dev \
    sqlite3 \
    curl && \
    apt-get autoremove -y && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install Crow (header-only, but building it sets up CMake config files)
RUN wget -O CrowCpp.zip https://github.com/CrowCpp/Crow/archive/refs/heads/master.zip && \
    unzip CrowCpp.zip && \
    cd Crow-master && \
    mkdir build && \
    cd build && \
    cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf CrowCpp.zip Crow-master

# Install nlohmann/json (header-only)
RUN git clone https://github.com/nlohmann/json.git /usr/src/nlohmann_json && \
    cd /usr/src/nlohmann_json && \
    cmake -S . -B build -DBUILD_TESTING=OFF && \
    cmake --install build && \
    rm -rf /usr/src/nlohmann_json

# (Optional) Install GoogleTest — remove if not writing tests inside container
RUN wget -O google-test.zip https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip && \
    unzip google-test.zip && \
    cd googletest-* && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf google-test.zip googletest-*

# Reset frontend
ENV DEBIAN_FRONTEND=

# Working directory for development
WORKDIR /workspace

# Default shell
CMD ["bash"]

