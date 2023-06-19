Para los ejercicios de mpi .

Instalar MPI:
-- Debian based --
sudo apt install libopenmpi-dev openmpi-doc
-- Arch base --
sudo pacman -S openmpi

Compilar:
mpicc program_name.c -o binary_file

Ejecutar:
mpirun -np [number of processes] ./binary_file




