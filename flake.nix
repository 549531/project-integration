{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    make-shell.url = "github:nicknovitski/make-shell";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "i686-linux"
        "x86_64-darwin"
        "aarch64-linux"
        "aarch64-darwin"
      ];

      imports = [
        ./api/default.nix
        ./docs/default.nix
        ./frontend/default.nix
        ./glove/default.nix
        inputs.make-shell.flakeModules.default
        inputs.treefmt-nix.flakeModule
      ];

      perSystem =
        { config, pkgs, ... }:
        {
          make-shells.default = {
            packages = [
              config.treefmt.build.wrapper
            ];
          };

          treefmt.programs = {
            nixfmt.enable = true;
            shfmt.enable = true;
            taplo.enable = true;
          };
        };
    };
}
