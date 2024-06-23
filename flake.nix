{
  description = "vulkan dev environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/24.05";
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
        ];
        VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
        shellHook = "zsh";

      };
    };
}
