################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Src/%.obj: ../Src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1120/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 --opt_for_speed=1 --include_path="C:/ptj/TI_LAUNCHXL_F28069M/Template/Template_TMS320F28069M" --include_path="C:/ti/c2000/C2000Ware_4_01_00_00/device_support/f2806x/common/include" --include_path="C:/ti/c2000/C2000Ware_4_01_00_00/device_support/f2806x/headers/include" --include_path="C:/ti/ccs1120/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Src/$(basename $(<F)).d_raw" --obj_directory="Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


