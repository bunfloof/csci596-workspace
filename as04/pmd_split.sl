#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=8
#SBATCH --time=00:04:59
#SBATCH --output=pmd_split.out
#SBATCH -A anakano_429

mpirun -n $SLURM_NTASKS ./pmd_split
