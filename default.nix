{ nixpkgs ? import <nixpkgs> {}
}:

with nixpkgs;

stdenv.mkDerivation {
  name = "plasma-i3-tool";
  src = ./.;
  buildInputs = [ cmake xorg.libX11 ];
}

