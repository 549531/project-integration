{
  perSystem = { pkgs, ... }: {
    make-shells.default = {
      packages =
        let
          py = pkgs.python3.withPackages (ps: [
            ps.furo
            ps.sphinx
            ps.sphinx-autobuild
            ps.sphinxcontrib-katex
            ps.sphinxcontrib-plantuml
            ps.sphinx-copybutton
          ]);
        in
        [
          pkgs.plantuml
          pkgs.nodejs
          py
        ];

      treefmt.programs = {
        black.enable = true;
      };
    };
  };
}
