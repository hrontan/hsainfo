#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <mutex>
#include "hsa.h"
#include "hsa_ext_finalize.h"

std::string data_size_to_string(size_t size){
	if (size < 1024) return std::to_string(size) + " B";
	else if (size < 1024*1024) return std::to_string(size/1024.0) + " KB";
	else if (size < 1024*1024*1024) return std::to_string(size/1024.0/1024) + " MB";
	else return std::to_string(size/1024.0/1024/1024) + " GB ";
}

static hsa_status_t get_agent(hsa_agent_t agent, void *data) {
	std::mutex mu;
	mu.lock();
	((std::vector<hsa_agent_t>*)data)->push_back(agent);
	mu.unlock();
	return HSA_STATUS_SUCCESS;
}

void print_hsa_agent(hsa_agent_t agent){

	char name[64] = { 0 };
	hsa_agent_get_info(agent, HSA_AGENT_INFO_NAME, &name);
	std::cout << "name : "  << name << std::endl;

	char vendor[64] = { 0 };
	hsa_agent_get_info(agent, HSA_AGENT_INFO_VENDOR_NAME, &vendor);
	std::cout << "vendor : "  << name << std::endl;

	hsa_agent_feature_t feature;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_FEATURE, &feature);
	std::cout << "feature : " << feature << " ( ";
	if (feature & HSA_AGENT_FEATURE_KERNEL_DISPATCH) std::cout << "KERNEL ";
	if (feature & HSA_AGENT_FEATURE_AGENT_DISPATCH) std::cout << "AGENT ";
	std::cout << ")" << std::endl;

	hsa_machine_model_t model;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_MACHINE_MODEL, &model);
	std::cout << "machine model : " << model;
	if (model == HSA_MACHINE_MODEL_SMALL) std::cout << " ( SMALL )";
	else if (model == HSA_MACHINE_MODEL_LARGE) std::cout << " ( LARGE )";
	else std::cout << " ( Not Defined )";
	std::cout << std::endl;

	hsa_profile_t profile;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_PROFILE, &profile);
	std::cout << "profile : " << profile;
	if (profile == HSA_PROFILE_BASE) std::cout << " ( BASE )";
	else if (profile == HSA_PROFILE_FULL) std::cout << " ( FULL )";
	else std::cout << " ( Not Defined )";
	std::cout << std::endl;

	hsa_default_float_rounding_mode_t default_float_rounding_mode;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_DEFAULT_FLOAT_ROUNDING_MODE, &default_float_rounding_mode);
	std::cout << "default float rounding mode : " << default_float_rounding_mode;
	if (default_float_rounding_mode == HSA_DEFAULT_FLOAT_ROUNDING_MODE_ZERO) std::cout << " ( ZERO )";
	else if (default_float_rounding_mode == HSA_DEFAULT_FLOAT_ROUNDING_MODE_NEAR) std::cout << " ( NEAR )";
	else std::cout << " ( Not Defined )";
	std::cout << std::endl;

	hsa_default_float_rounding_mode_t base_profile_float_rounding_modes;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_BASE_PROFILE_DEFAULT_FLOAT_ROUNDING_MODES, &base_profile_float_rounding_modes);
	std::cout << "base profile float rounding modes : " << base_profile_float_rounding_modes;
	if (base_profile_float_rounding_modes == HSA_DEFAULT_FLOAT_ROUNDING_MODE_ZERO) std::cout << " ( ZERO )";
	else if (base_profile_float_rounding_modes == HSA_DEFAULT_FLOAT_ROUNDING_MODE_NEAR) std::cout << " ( NEAR )";
	else std::cout << " ( Not Defined )";
	std::cout << std::endl;

	bool fast_f16_operation;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_FAST_F16_OPERATION, &fast_f16_operation);
	std::cout << "fast f16 operation : " << std::boolalpha << fast_f16_operation << std::endl;

	uint32_t wavefront_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_WAVEFRONT_SIZE, &wavefront_size);
	std::cout << "wavefront size : " << wavefront_size << std::endl;

	uint16_t workgroup_max_dim[3];
	hsa_agent_get_info(agent, HSA_AGENT_INFO_WORKGROUP_MAX_DIM, &workgroup_max_dim);
	std::cout << "workgroup max dim : { " << workgroup_max_dim[0] << ", " << workgroup_max_dim[1] << ", " << workgroup_max_dim[2] << " }" << std::endl;

	uint32_t workgroup_max_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_WORKGROUP_MAX_SIZE, &workgroup_max_size);
	std::cout << "workgroup max size : " << workgroup_max_size << std::endl;

	hsa_dim3_t grid_max_dim;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_GRID_MAX_DIM, &grid_max_dim);
	std::cout << "grid max dim : { " << grid_max_dim.x << ", " << grid_max_dim.y << ", " << grid_max_dim.z << " }" << std::endl;

	uint32_t grid_max_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_GRID_MAX_SIZE, &grid_max_size);
	std::cout << "grid max size : " << grid_max_size << std::endl;

	uint32_t fbarrier_max_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_FBARRIER_MAX_SIZE, &fbarrier_max_size);
	std::cout << "fbarrier max size : " << fbarrier_max_size << std::endl;

	uint32_t queues_max;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_QUEUES_MAX, &queues_max);
	std::cout << "queues max : " << queues_max << std::endl;

	uint32_t queue_min_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_QUEUE_MIN_SIZE, &queue_min_size);
	std::cout << "queue min size : " << queue_min_size << std::endl;

	uint32_t queue_max_size;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_QUEUE_MAX_SIZE, &queue_max_size);
	std::cout << "queue max size : " << queue_max_size << std::endl;

	hsa_queue_type_t queue_type;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_QUEUE_TYPE, &queue_type);
	std::cout << "queue type : " << queue_type;
	if (queue_type == HSA_QUEUE_TYPE_MULTI) std::cout << " ( MULTI ) ";
	else if (queue_type == HSA_QUEUE_TYPE_SINGLE) std::cout << " ( SINGLE ) ";
	else std::cout << " ( Not Defined ) ";
	std::cout << std::endl;

	uint32_t node;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_NODE, &node);
	std::cout << "node : " << node << std::endl;

	hsa_device_type_t device;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE, &device);
	std::cout << "device : "  << device;
	if (device == HSA_DEVICE_TYPE_CPU) std::cout << " ( CPU ) ";
	else if (device == HSA_DEVICE_TYPE_GPU) std::cout << " ( GPU ) ";
	else if (device == HSA_DEVICE_TYPE_DSP) std::cout << " ( DSP ) ";
	else std::cout << " ( Not Defined ) ";
	std::cout << std::endl;

	uint32_t cache_size[4];
	hsa_agent_get_info(agent, HSA_AGENT_INFO_CACHE_SIZE, &cache_size);
	std::cout << "cache size : { L1 : "  << cache_size[0]  << ", L2 : " << cache_size[1] << ", L3 : " << cache_size[2] << ", L4 : " << cache_size[3] << " }"<< std::endl;

	hsa_isa_t isa;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_ISA, &isa);
	std::cout << "isa : " << isa.handle  << std::endl;

	uint8_t extensions[128];
	hsa_agent_get_info(agent, HSA_AGENT_INFO_EXTENSIONS, &extensions);
	std::cout << "extensions : ";
	for(int i = 0; i < 128; i++) std::cout << extensions[i];
	std::cout << std::endl;

	uint16_t version_mejor;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_VERSION_MAJOR, &version_mejor);
	std::cout << "version mejor : "  << version_mejor << std::endl;

	uint16_t version_minor;
	hsa_agent_get_info(agent, HSA_AGENT_INFO_VERSION_MINOR, &version_minor);
	std::cout << "version minor : "  << version_minor << std::endl;

}

static hsa_status_t find_memory_region(hsa_region_t region, void* data){
	std::mutex mu;
	mu.lock();
	((std::vector<hsa_region_t>*)data)->push_back(region);
	mu.unlock();
	return HSA_STATUS_SUCCESS;
}

void print_hsa_region(hsa_region_t region){
		hsa_region_segment_t segment;
		hsa_region_get_info(region, HSA_REGION_INFO_SEGMENT, &segment);
		std::cout << "segment : " << segment;
		if ( segment == HSA_REGION_SEGMENT_GLOBAL ) std::cout << " ( GLOBAL )";
		else if ( segment == HSA_REGION_SEGMENT_READONLY ) std::cout << " ( READONLY )";
		else if ( segment == HSA_REGION_SEGMENT_PRIVATE ) std::cout << " ( PRIVATE )";
		else if ( segment == HSA_REGION_SEGMENT_GROUP ) std::cout << " ( GROUP )";
		else std::cout << "( Not Defined )";
		std::cout << std::endl;

		uint32_t global_flags;
		hsa_region_get_info(region, HSA_REGION_INFO_GLOBAL_FLAGS, &global_flags);
		std::cout << "global flags : " << global_flags << " ( ";
		if ( global_flags & HSA_REGION_GLOBAL_FLAG_KERNARG ) std::cout << "KERNARG ";
		if ( global_flags & HSA_REGION_GLOBAL_FLAG_FINE_GRAINED ) std::cout << "FINEGRAINED ";
		if ( global_flags & HSA_REGION_GLOBAL_FLAG_COARSE_GRAINED ) std::cout << "COARSEGRAINED ";
		std::cout << ")" << std::endl;

		size_t size;
		hsa_region_get_info(region, HSA_REGION_INFO_SIZE, &size);
		std::cout << "size : " << size << " ( " << data_size_to_string(size) << " )"  << std::endl;

		size_t alloc_max_size;
		hsa_region_get_info(region, HSA_REGION_INFO_ALLOC_MAX_SIZE, &alloc_max_size);
		std::cout << "alloc max size : " << alloc_max_size << " ( " << data_size_to_string(alloc_max_size) << " )" << std::endl;

		bool runtime_alloc_allowd;
		hsa_region_get_info(region, HSA_REGION_INFO_RUNTIME_ALLOC_ALLOWED, &runtime_alloc_allowd);
		std::cout << "runtime alloc allowed : " << std::boolalpha << runtime_alloc_allowd << std::endl;

		size_t runtime_alloc_granule;
		hsa_region_get_info(region, HSA_REGION_INFO_RUNTIME_ALLOC_GRANULE, &runtime_alloc_granule);
		std::cout << "runtime alloc granule : " << runtime_alloc_granule << " ( " << data_size_to_string(runtime_alloc_granule) << " )" << std::endl;

		size_t runtime_alloc_alignment;
		hsa_region_get_info(region, HSA_REGION_INFO_RUNTIME_ALLOC_ALIGNMENT, &runtime_alloc_alignment);
		std::cout << "runtime alloc alignment : " << runtime_alloc_alignment << " ( "<< data_size_to_string(runtime_alloc_alignment)<< " )"<< std::endl;

}

int main(int argc, char *argv[]) {

	hsa_init();
	std::vector<hsa_agent_t> agents;
	hsa_iterate_agents(get_agent, &agents);

	for(unsigned int i = 0; i < agents.size(); i++){
		std::cout << "= Device " << i << std::endl;
		print_hsa_agent(agents[i]);
		std::vector<hsa_region_t> regions;
		hsa_agent_iterate_regions(agents[i], find_memory_region, &regions);
		for(unsigned int j = 0; j< regions.size(); j++){
			std::cout << "** Region " << j << std::endl;
			print_hsa_region(regions[j]);
		}
		std::cout << std::endl;
	}

	hsa_shut_down();

	return 0;
}
