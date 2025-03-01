{
  inputs = {
    devshell.inputs.nixpkgs.follows = "nixpkgs";
    devshell.url = "github:numtide/devshell";
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" "i686-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];

      imports = [
        inputs.devshell.flakeModule
        inputs.treefmt-nix.flakeModule
      ];

      perSystem = { config, pkgs, ... }: {
        devshells.default = {
          packages = [
            config.treefmt.build.wrapper
          ];
        };

        treefmt.programs = {
          nixpkgs-fmt.enable = true;
          shfmt.enable = true;
          taplo.enable = true;
          yamlfmt.enable = true;
        };
      };
    };
}
