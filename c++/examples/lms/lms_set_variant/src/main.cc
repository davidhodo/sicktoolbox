/*!
 * \file main.cc
 * \brief Illustrates how to set the device variant and then
 *        acquire measured values
 *
 * Note: This example WILL NOT WORK for LMS 211-S14, 221-S14,
 *       291-S14 models as they do not support variant switching.
 *
 * Code by Jason C. Derenick and Thomas H. Miller.
 * Contact derenick(at)lehigh(dot)edu
 *
 * The Sick LIDAR Matlab/C++ Toolbox
 * Copyright (c) 2008, Jason C. Derenick and Thomas H. Miller
 * All rights reserved.
 *
 * This software is released under a BSD Open-Source License.
 * See http://sicktoolbox.sourceforge.net
 */

/* Implementation dependencies */
#include <string>
#include <iostream>
#include <sicklms-1.0/SickLMS.hh>

/* Use the namespace */
using namespace SickToolbox;

int main(int argc, char * argv[]) {
  
  std::string device_str;                
  SickLMS::sick_lms_baud_t desired_baud = SickLMS::SICK_BAUD_38400;
  
  /* Check for a device path.  If it's not present, print a usage statement. */
  if ((argc != 2 && argc != 3) || (argc == 2 && strcasecmp(argv[1],"--help") == 0)) {
    std::cout << "Usage: lms_set_variant PATH [BAUD RATE]" << std::endl
	      << "Ex: lms_set_variant /dev/ttyUSB0 9600" << std::endl;
    return -1;
  }

  /* Only device path is given */
  if (argc == 2) {
    device_str = argv[1];
  }

  /* Device path and baud are given */
  if (argc == 3) {    
    device_str = argv[1];
    if ((desired_baud = SickLMS::StringToSickBaud(argv[2])) == SickLMS::SICK_BAUD_UNKNOWN) {
      std::cerr << "Invalid baud value! Valid values are: 9600, 19200, 38400, and 500000" << std::endl;
      return -1;
    }
  }

  /* Instantiate the SickLMS class with the device path string. */
  SickLMS sick_lms(device_str);

  /* Define some buffers to hold the returned measurements */
  unsigned int values[SickLMS::SICK_MAX_NUM_MEASUREMENTS] = {0};
  unsigned int num_values = 0;
  
  try {

    /*
     * Attempt to initialize the device
     */
    sick_lms.Initialize(desired_baud);

    /*
     * Set the device variant to 100/0.25
     *
     * NOTE: If an invalid variant definition is
     *       given a SickConfigException will be
     *       thrown stating so.
     *
     */
    std::cout << "\tSetting variant to 100/0.25" << std::endl << std::flush;
    sick_lms.SetSickVariant(SickLMS::SICK_SCAN_ANGLE_100,SickLMS::SICK_SCAN_RESOLUTION_25);
    
    /*
     * Acquire some measurements from Sick LMS 2xx using 100/0.25
     */
    std::cout << "\tAcquiring some measurements..." << std::endl << std::flush;
    for(unsigned int i = 0; i < 10; i++) {

      try {
      
	/* Acquire the most recent scan from the Sick */
	sick_lms.GetSickScan(values,num_values);

	/* Display the number of measurements */
	std::cout << "\t  Num. Values: " << num_values << std::endl << std::flush;
	
      }

      /* Ignore a timeout if it occurs */
      catch(SickTimeoutException &sick_timeout) {
	std::cerr << sick_timeout.what() << std::endl;
      }

      /* Catch and rethrow any other Sick exception */
      catch(SickException &sick_exception) {
	std::cerr << sick_exception.what() << std::endl;
	throw;
      }

    }

    /*
     * Set the device variant to 180/0.5
     */
    std::cout << std::endl << "\tSetting variant to 180/0.50" << std::endl;
    sick_lms.SetSickVariant(SickLMS::SICK_SCAN_ANGLE_180,SickLMS::SICK_SCAN_RESOLUTION_50);
    
    /*
     * Acquire some measurements from Sick LMS 2xx using 180/0.50
     */
    std::cout << "\tAcquiring some measurements..." << std::endl << std::flush;
    for(unsigned int i = 0; i < 10; i++) {

      try {
      
	/* Acquire the most recent scan from the Sick */
	sick_lms.GetSickScan(values,num_values);

	/* Display the number of measured values */
	std::cout << "\t  Num. Values: " << num_values << std::endl << std::flush;
	
      }

      /* Ignore a timeout if it occurs */
      catch(SickTimeoutException &sick_timeout) {
	std::cerr << sick_timeout.what() << std::endl;
      }

      /* Throw away anything else */
      catch(...) {
	throw;
      }

    }
    
    /*
     * Attempt to uninitialize the device
     */
    sick_lms.Uninitialize();
    
  }

  /* Bail for any other exception */
  catch(...) {
    std::cerr << "An error occurred!" << std::endl;
    return -1;
  }

  std::cout << "Done!!! :o)" << std::endl;
  
  /* Success! */
  return 0;

}