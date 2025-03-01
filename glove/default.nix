{
  perSystem = { pkgs, ... }: {
    devshells.default = {
      packages = [
        pkgs.clang-tools
        pkgs.cmake-format
        pkgs.nixpkgs-fmt
        pkgs.platformio
      ];

      env = [
        { name = "ESPBAUD"; value = 921600; }
      ];
    };

    treefmt.programs = {
      clang-format.enable = true;
      cmake-format.enable = true;
    };
  };
}
