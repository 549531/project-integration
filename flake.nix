{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    esp-idf.url = "github:mirrexagon/nixpkgs-esp-dev";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" "i686-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];

      perSystem = { config, self', inputs', pkgs, system, ... }: {

        devShells.default = pkgs.mkShell {

          packages = [
            inputs'.esp-idf.packages.esp-idf-esp32
            pkgs.clang-tools
            pkgs.cmake-format
            pkgs.nixpkgs-fmt
            pkgs.shfmt
            pkgs.taplo
            pkgs.treefmt
            pkgs.yamlfmt
          ];

        };
      };
    };
}
