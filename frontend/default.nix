{
  perSystem =
    { pkgs, ... }:
    {
      make-shells.default = {
        packages = [
          pkgs.yarn-berry
        ];

      };

      treefmt.programs = {
        prettier.enable = true;
      };
    };
}
