#include "vehicleregistry.h"


// Default Constructor
VehicleRegistry::VehicleRegistry()
{
	maxsize = 4;
	vehicles = new Vehicle[maxsize];
	numvehicles = 0;
}

// Copy Constructor
VehicleRegistry::VehicleRegistry(const VehicleRegistry& vr)
{
	CopyArray(vr);
}

// Destructor
VehicleRegistry::~VehicleRegistry()
{
	delete[] vehicles;
}

//Helper method
/*Makes another array with the same values in dynmaic memory with a different address*/
void VehicleRegistry::CopyArray(const VehicleRegistry& vr)
{
	vehicles = new Vehicle[vr.maxsize];
	maxsize = vr.maxsize;
	numvehicles = vr.numvehicles;

	for (int i = 0; i < numvehicles; i++)
	{
		vehicles[i].UpdatePlate(vr.vehicles[i].GetPlate());
		vehicles[i].UpdateMake(vr.vehicles[i].GetMake());
		vehicles[i].UpdateModel(vr.vehicles[i].GetModel());
		vehicles[i].UpdateColour(vr.vehicles[i].GetColour());
	}
}

	//////////////////////////
    /////// Mutators /////////
    //////////////////////////
bool VehicleRegistry::Insert(Vehicle vehicle)
{
	for (int i = 0; i < numvehicles; i++)
	{
		if (vehicles[i].GetPlate() == vehicle.GetPlate())
		{
			cout << "Failed to insert vehicle, same plate number exists in registry." << endl;
			return false;
		}
	}

	if (numvehicles == maxsize) //expand array
	{
		Vehicle *old_arr = vehicles;
		maxsize = 2*maxsize;
		vehicles = new Vehicle[maxsize];

		for (int i = 0; i < numvehicles; i++)
		{
			vehicles[i].UpdatePlate(old_arr[i].GetPlate());
			vehicles[i].UpdateMake(old_arr[i].GetMake());
			vehicles[i].UpdateModel(old_arr[i].GetModel());
			vehicles[i].UpdateColour(old_arr[i].GetColour());
		}

		delete [] old_arr;
	}

	vehicles[numvehicles].UpdatePlate(vehicle.GetPlate()); //this records the plate number and saves it in the index of vehicles array
	vehicles[numvehicles].UpdateMake(vehicle.GetMake());
	vehicles[numvehicles].UpdateModel(vehicle.GetModel());
	vehicles[numvehicles].UpdateColour(vehicle.GetColour());

	numvehicles += 1;

	return true;
}

/*If all parameters match, update the target vehicle with the information of the numvehicle-1 index and decrease the number of vehicles in the registry by 1
Updating the target vehicle is basically removing the target vehicle*/
bool VehicleRegistry::Remove(Vehicle vehicle)
{
	for (int i = 0; i < numvehicles; i++)
	{
		if ( vehicles[i].GetPlate() == vehicle.GetPlate() && vehicles[i].GetMake() == vehicle.GetMake() && vehicles[i].GetModel() == vehicle.GetModel() && vehicles[i].GetColour() == vehicle.GetColour())
		{
			vehicles[i].Update(vehicles[numvehicles-1].GetPlate(),vehicles[numvehicles-1].GetMake(),vehicles[numvehicles-1].GetModel(),vehicles[numvehicles-1].GetColour());
			vehicles[numvehicles-1].Update("clear","clear","clear","clear");
			numvehicles -= 1;
			return true;
		}
	}

	cout << "could not remove vehicle" << endl;
	return false;
}


	//////////////////////////
    /////// Accessors ////////
    //////////////////////////
/*As long as there is a matching plate number it will return the index*/
int VehicleRegistry::Find(Vehicle vehicle) const
{
	for (int i = 0; i < numvehicles; i++)
	{
		if (vehicles[i].GetPlate() == vehicle.GetPlate())
		{
			return i;
		}
	}

	return -1;

}

/*Display the number of vehicles in a specificed registry when called*/
int VehicleRegistry::Count() const
{
	return numvehicles;
}


	/////////////////////////
	///// Set Operators /////
	/////////////////////////
/*Performs the 'union' set operator*/
VehicleRegistry VehicleRegistry::Join(const VehicleRegistry& vr) const  // a function that returns an object
{
	
	VehicleRegistry unions(*this); //keyword 'this' is the pointer to the 'calling' object

	/*Go through the vr list, and use the find function with union as the calling object, if we can't find it in the union list then we add the vehicle to the union list */
	for (int i = 0; i < vr.numvehicles; i++)
	{		
		if (unions.Find(Vehicle(vr.vehicles[i].GetPlate(), "", "", "")) == -1)
		{
			unions.Insert(Vehicle(vr.vehicles[i].GetPlate(), vr.vehicles[i].GetMake(), vr.vehicles[i].GetModel(), vr.vehicles[i].GetColour()));
		}
	}

	return unions;
}

/*Performs the 'intersection' set operator*/
VehicleRegistry VehicleRegistry::Common(const VehicleRegistry& vr) const
{
	VehicleRegistry common(*this); //keyword 'this' is the pointer to the 'calling' object


	//Use a double for loop to go through both list and if matching parameters are found we leave it alone, if we find non-matching parameters after going through the entire list 
	//then remove the item
	for (int i = 0; i < common.numvehicles; i++)
	{
		for (int p = 0; p < vr.numvehicles; p++)
		{

			if (p < vr.numvehicles && common.vehicles[i].GetPlate() == vr.vehicles[p].GetPlate())
			{
				break;
			}
			else if (p == (vr.numvehicles - 1) && common.vehicles[i].GetPlate() != vr.vehicles[p].GetPlate())
			{
				common.Remove(Vehicle(common.vehicles[i].GetPlate(), common.vehicles[i].GetMake(), common.vehicles[i].GetModel(), common.vehicles[i].GetColour()));
				i = 0;
			}
		}
	}

	return common;
}


/*Performs the 'difference' set operator*/
VehicleRegistry VehicleRegistry::Split(const VehicleRegistry& vr) const
{
	VehicleRegistry differences(*this);

	//Go through the vr list and use the find function with union as the calling object, if we do find it in the union list then we remove the vehicle from the union list
	for (int i = 0; i < vr.numvehicles; i++)
	{
		if (differences.Find(Vehicle(vr.vehicles[i].GetPlate(), "", "", "")) != -1)
		{
			differences.Remove(Vehicle(vr.vehicles[i].GetPlate(), vr.vehicles[i].GetMake(), vr.vehicles[i].GetModel(), vr.vehicles[i].GetColour()));
		}
	}

	return differences;

}