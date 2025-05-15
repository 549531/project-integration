{
  perSystem = { pkgs, ... }: {
    make-shells.default = {
      packages = [
        pkgs.clang-tools
        pkgs.platformio
      ];
    };

    treefmt.programs = {
      black.enable = true;
      clang-format.enable = true;
      cmake-format.enable = true;
    };
  };
}
