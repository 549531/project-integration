# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Project Integration'
author = ('Anton Bucataru, '
          'Anton Tsushko, '
          'Dmytro Khorsun, '
          'Obidkhon Akhmadkhonov, '
          'Rewend Al-Gaaf')
copyright = f'2025 {author}'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinxcontrib.katex',
    'sphinxcontrib.plantuml',
    'sphinx_copybutton',
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

nitpicky = True

# -- Options for plugins -----------------------------------------------------

katex_prerender = True
plantuml_batch_size = 10
plantuml_output_format = 'svg_img'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'furo'
html_static_path = ['_static']

html_theme_options = {
    'source_repository': 'https://github.com/549531/project-integration',
    'source_branch': 'main',
    'source_directory': 'docs',
}
