#include "../Headers/obj.hpp"
#include "../Headers/radarDefinition.hpp"
#include "../Headers/ExtendedObjectDefinition.hpp"
#include "../Headers/declarations.hpp"

/**
 * @brief Update the state of tracked, untracked objects and explores the posibility of detecting new objects based on incoming measurements using ellipsoidal gating, PPP update and MB update.
 *
 * This function performs the update step in the tracking process. It takes a collection of tracked and untracked objects, incoming measurements,
 * radar configuration, and extended object definition as input. The function first creates a measurements structure using the provided data,
 * then applies ellipsoidal gating to identify measurements within the Mahalanobis distance from the tracked objects. Later it updates the state
 * of untracked objects and examine wether any object got detected or not. Finally, the states of the tracked objects 
 * get updated.
 *
 * @param collection An ObjectsCollection reference containing the current state of tracked and untracked objects.
 * @param data A Matrix pointer representing the incoming measurements.
 * @param radar A radarDefinition pointer representing the radar configuration.
 * @param extObj An ExtendedObjectDefinition pointer providing information about the extended object model.
 *
 * @note The function internally creates a measurements structure based on the incoming data and performs ellipsoidal gating to determine
 *       which measurements are within the Mahalanobis distance from the tracked objects. The tracked objects are then updated using the
 *       PPP update method.
 */
void update(ObjectsCollection& collection,
            Matrix* data,
            radarDefinition* radar,
            ExtendedObjectDefinition* extObj){
               std::cout << "Started the update step ..." << std::endl;
               measurements currentMeasurements(data);
               std::vector<TrackedObj> tempppppp = collection.MB;
               elipsoidalGating(radar, &collection,'M', currentMeasurements);

               PPP_update(collection,
                          &currentMeasurements,
                          radar,
                          extObj);
               std::cout << collection.MB.size();
               std::cout << collection.MB[2];

               MB_update(collection,
                         &currentMeasurements,
                         radar,
                         extObj);
            };


