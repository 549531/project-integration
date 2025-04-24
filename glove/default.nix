{
  perSystem = { pkgs, ... }: {
    make-shells.default = {
      packages = [
        pkgs.clang-tools
        pkgs.cmake-format
        pkgs.nixpkgs-fmt
        pkgs.platformio
      ];

      env.ESPBAUD = 921600;
    };

    treefmt.programs = {
      clang-format.enable = true;
      cmake-format.enable = true;
    };
  };
}
