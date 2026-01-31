FROM devkitpro/devkita64 AS builder

# makepkg requires fakeroot; install via apt since base image is Debian
RUN apt-get update && apt-get install -y --no-install-recommends \
    fakeroot \
    file \
    zstd \
    && rm -rf /var/lib/apt/lists/*

# Build deps from the PKGBUILD (plus base-devel for makepkg)
RUN dkp-pacman -Syu --noconfirm --needed \
    switch-pkg-config dkp-toolchain-vars

RUN useradd -m build && mkdir -p /build && chown -R build:build /build
USER build
WORKDIR /build

COPY PKGBUILD ./

ENV PATH=/opt/devkitpro/pacman/bin:$PATH
ENV MAKEFLAGS=-j4
RUN makepkg -s --noconfirm --cleanbuild

FROM devkitpro/devkita64

COPY --from=builder /build/*.pkg.tar.* /tmp/
RUN dkp-pacman -U --noconfirm /tmp/*.pkg.tar.* && rm -f /tmp/*.pkg.tar.*
