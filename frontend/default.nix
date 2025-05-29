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
        prettier.excludes = [ "*/yarn-*.cjs" ];
      };
    };
}
