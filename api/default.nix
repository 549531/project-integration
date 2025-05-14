{
  perSystem = { pkgs, ... }: {
    make-shells.default = {
      packages = [
        pkgs.uv
      ];
    };

    treefmt.programs = {
      black.enable = true;
    };
  };
}
