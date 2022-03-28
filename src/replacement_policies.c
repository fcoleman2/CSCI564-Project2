//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"
#include <limits.h>


// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary
int global_count = 0;
struct metadata
{
    
    int *use_number;
   
};



void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the LRU replacement policy state given a new memory access

    // Loop through the set index and see if there is a matching tag
    for(int i=0; i< cache_system->associativity;i++){
       
        // Check for matching tag
        if(cache_system->cache_lines[set_idx * cache_system -> associativity + i].tag == tag){
       

          //cache_system->cache_lines +set_idx*cache_system->associativity+i)
          ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i] = global_count;
         
        }
        
    }
    global_count++;
   
    
    
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
   // int line_idx;

    int oldest = 0;
    int return_index=0;

  for(int i=0; i<cache_system->associativity;i++){
       // set oldest to the first value
       if(oldest ==0){
           oldest = ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i];
       }
       if( oldest > ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i] ){
           
           oldest = ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i];
           return_index = i;

       }
      

   }

    
    return return_index;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.

    free ((( struct metadata *) replacement_policy -> data)-> use_number);
    free ((( struct metadata *) replacement_policy -> data));
    
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data.
    // Allocate memory for metadata
    struct metadata *information = malloc(sizeof(struct metadata));
   // information -> global_count =0;
    
    //Allocate the array space needed 
    information -> use_number = calloc(sets *associativity,sizeof(int));
    
    lru_rp->data =  information;

    //int order [sets][associativity] ;
    //lru_rp -> data = &order; 
    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the RAND replacement policy state given a new memory access
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.

    return (rand() % cache_system->associativity) ;
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.

    
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;
    //rand_rp -> data = malloc(sizeof());

    // TODO allocate any additional memory to store metadata here and assign to
    // rand_rp->data.

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy

      for(int i=0; i< cache_system->associativity;i++){
       
        // Check for matching tag
        if(cache_system->cache_lines[set_idx * cache_system -> associativity + i].tag == tag){
       // if(((cache_system->cache_lines +set_idx *cache_system->associativity + i) ->tag ==tag){
          // Update to the most recent time
        

      
          ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i] = global_count;
        
        }
        
    }
    global_count++;
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.


    // Don't pick anything that is Modified

    //cache_system->cache_lines[set_idx*cache_system->associativity+ i].status !=MODIFIED;

    int oldest = INT_MAX; 
    int result_index=-1; 

   for(int i=0; i<cache_system->associativity;i++){
   
       if( oldest > ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i] && cache_system->cache_lines[set_idx*cache_system->associativity+ i].status !=MODIFIED ){
           
           oldest = ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i];
           result_index = i;

       }
   }
   if(result_index==-1){
       for(int i=0; i<cache_system->associativity;i++){
   
       
           if( oldest > ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i]  ){
           
            oldest = ((struct metadata *) replacement_policy ->data)->use_number[set_idx *cache_system->associativity + i];
            result_index = i;

           }   
       }

   }

    return  result_index;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
    free ((( struct metadata *) replacement_policy -> data)-> use_number);
    free ((( struct metadata *) replacement_policy -> data));
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
  
    struct metadata *information = malloc(sizeof(struct metadata));
    //int order[sets * associativity];
    information -> use_number = calloc(sets *associativity,sizeof(int));
    lru_prefer_clean_rp->data =  information;

    return lru_prefer_clean_rp;
}