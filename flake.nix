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
          hello
          glfw

          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-validation-layers
        ];
        shellHook = "zsh";
      };
    };
}
