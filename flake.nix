{
  description = "vulkan dev environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          cmake
          glfw

          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-validation-layers

          glm
          shaderc

          cloc
        ];
        VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
        shellHook = "zsh";

      };
    };
}
