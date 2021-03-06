//
//  KBDataStore.h
//  KegPad
//
//  Created by Gabriel Handford on 7/28/10.
//  Copyright 2010 Yelp. All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#import <CoreData/CoreData.h>

#import "KBKeg.h"
#import "KBUser.h"
#import "KBBeer.h"
#import "KBKegProcessing.h"
#import "KBTypes.h"
#import "KBKegTimeHost.h"


#define KBDataStoreCheckError(__ERROR__) do { \
  if (error) NSLog(@"Error: %@; %@", error, [error localizedFailureReason]); \
} while(0)

#define KBOrNSNull(__OBJ__) (__OBJ__ ? __OBJ__ : (id)[NSNull null])


/*!
 Main data store (CoreData).
 */
@interface KBDataStore : NSObject {
  
@private
  NSManagedObjectContext *managedObjectContext_;
  NSManagedObjectModel *managedObjectModel_;
  NSPersistentStoreCoordinator *persistentStoreCoordinator_;
  
  NSString *name_;
  NSString *path_;
}

- (id)initWithName:(NSString *)name path:(NSString *)path;

/*!
 Save.
 @param error Out error
 @result YES if saved
 */
- (BOOL)save:(NSError **)error;

/*!
 Returns the managed object context for the application.
 If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
 */
- (NSManagedObjectContext *)managedObjectContext;

/*!
 Returns the managed object model for the application.
 If the model doesn't already exist, it is created from the application's model.
 */
- (NSManagedObjectModel *)managedObjectModel;

/*!
 Returns the persistent store coordinator for the application.
 If the coordinator doesn't already exist, it is created and the application's store added to it.
 */
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator;

/*!
 Documents directory, where the data store is persisted.
 */
- (NSString *)applicationDocumentsDirectory;

/*!
 Create object for entity.
 @param entityName Name of model class
 @param managedObjectContext Managed object context, or nil if not managed
 */
- (id)insertNewObjectForEntityForName:(NSString *)entityName inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

/*!
 Execute fetch
 @param request
 @param error Out error
 @result Results
 */
- (NSArray *)executeFetchRequest:(NSFetchRequest *)request error:(NSError **)error;

/*!
 Save context.
 @param error Out error
 @result YES if saved
 */
- (BOOL)save:(NSError **)error;

/*!
 Set the keg at position.
 */
- (void)setKeg:(KBKeg *)keg position:(NSInteger)position;

/*!
 Get the key at position.
 */
- (KBKeg *)kegAtPosition:(NSInteger)position;

/*!
 Get beer with identifier.
 @param id identifier
 @param error Out error
 @result Beer
 */
- (KBBeer *)beerWithId:(NSString *)id error:(NSError **)error;

/*!
 Get list of beers.
 @param offset
 @param limit
 @param error Out error
 @result Beers
 */
- (NSArray */*of KBBeer*/)beersWithOffset:(NSUInteger)offset limit:(NSUInteger)limit error:(NSError **)error;

/*!
 Add a keg with beer.
 @param beer Beer
 @param error Out error
 @result YES if keg was added
 */
- (BOOL)addKegWithBeer:(KBBeer *)beer error:(NSError **)error;

/*!
 Add or update beer with identifier.
 @param id Identifier
 @param name Name ("Stella Artois")
 @param info Description to set ("A pilsner ...")
 @param type Beer type ("Lager / Pilsner")
 @param country Country description ("Belgium")
 @param imageName Name of image in resources
 @param abv Alchohol by volume
 @param error Out error
 @result New beer if saved, nil otherwise
 */
- (KBBeer *)addOrUpdateBeerWithId:(NSString *)id name:(NSString *)name info:(NSString *)info type:(NSString *)type country:(NSString *)country 
imageName:(NSString *)imageName abv:(float)abv error:(NSError **)error;

/*!
 Add a keg temperature value for current date.
 @param temperature Temperature in celcius
 @param keg Keg
 @param error Out error
 @result YES if keg temperature was added
 */
- (KBKegTemperature *)addKegTemperature:(float)temperature keg:(KBKeg *)keg error:(NSError **)error;

/*!
 Add a keg pour.
 @param amount Amount in liters
 @param keg Keg
 @param user User, if nil is anonymous
 @param error Out error
 @result Keg pour that was added
 */
- (KBKegPour *)addKegPour:(float)amount keg:(KBKeg *)keg user:(KBUser *)user date:(NSDate *)date error:(NSError **)error;

/*!
 Get a list of recent keg pours.
 @param limit Max number of pours
 @param ascending Ascending
 @param error Out error
 @result List of keg pours
 */
- (NSArray */*of KBKegPour*/)recentKegPoursWithLimit:(NSUInteger)limit ascending:(BOOL)ascending error:(NSError **)error;

/*!
 Get last keg pour.
 @param error Out error
 @result Last keg pour
 */
- (KBKegPour *)lastPour:(NSError **)error;

/*!
 Add amount to existing pour.
 @param amount Amount to add
 @param pour Pour to add to
 @param error Out error
 */
- (BOOL)addAmount:(float)amount toPour:(KBKegPour *)pour error:(NSError **)error;

/*!
 Add/update a user.
 @param tagId Tag id, such as a RFID
 @param firstName First name
 @param lastName Last name
 @param isAdmin Whether user is admin
 @param error Out error
 @result User if saved, nil otherwise
 */
- (KBUser *)addOrUpdateUserWithTagId:(NSString *)tagId firstName:(NSString *)firstName lastName:(NSString *)lastName 
                             isAdmin:(BOOL)isAdmin error:(NSError **)error;

/*!
 Find user by tag id.
 @param tagId Tag id, such as a RFID
 @param error Out error
 */
- (KBUser *)userWithTagId:(NSString *)tagId error:(NSError **)error;

/*!
 Get a list of kegs.
 @param offset Offset
 @param limit limit
 @param error Out error
 @result List of kegs
 */
- (NSArray */*of KBKeg*/)kegsWithOffset:(NSUInteger)offset limit:(NSUInteger)limit error:(NSError **)error;

/*!
 Keg by id.
 @param id Identifier
 @param error Out error
 @result Keg
 */
- (KBKeg *)kegWithId:(NSString *)id error:(NSError **)error;

/*!
 Add/update keg information for id.
 @param id
 @param beer
 @param volumeAdjusted Volume in liters to adjust (subtract) the keg volume; This value with poured value and total value determines how much is remaining
 @param volumeTotal
 @param error Out error
 @result Keg if saved, nil otherwise
 */
- (KBKeg *)addOrUpdateKegWithId:(NSString *)id beer:(KBBeer *)beer volumeAdjusted:(float)volumeAdjusted volumeTotal:(float)volumeTotal error:(NSError **)error;

/*!
 Rating on beer for user.
 @param user
 @param beer
 @param error
 @result Rating
 */
- (KBRating *)ratingWithUser:(KBUser *)user beer:(KBBeer *)beer error:(NSError **)error;

/*!
 Set rating on beer for user.
 @param rating
 @param user
 @param keg
 @param error Out error
 @result Rating
 */
- (KBRating *)setRating:(KBRatingValue)ratingValue user:(KBUser *)user keg:(KBKeg *)keg error:(NSError **)error;

/*!
 Get list of users.
 @param offset Offset
 @param limit Limit
 @param error Out error
 @result List of users
 */
- (NSArray *)usersWithOffset:(NSUInteger)offset limit:(NSUInteger)limit error:(NSError **)error;

/*!
 Get top users (by pour).
 @param offset Offset
 @param limit Limit
 @param error Out error
 */
- (NSArray *)topUsersByPourWithOffset:(NSUInteger)offset limit:(NSUInteger)limit error:(NSError **)error;

/*!
 Get recent keg pours from date range for user (or all users).
 @param fromData
 @param toDate
 @param user User, if nil then will query all users
 @param error Out error
 @result List of keg pours
 */
- (NSArray */*of KBKegPour*/)recentKegPoursFromDate:(NSDate *)fromDate toDate:(NSDate *)toDate user:(KBUser *)user error:(NSError **)error;

/*!
 Get the rate for keg pours in the last hour for user.
 @deprecated
 @param user User, if nil will query for all users
 @param error Out error
 */
- (float)rateForKegPoursLastHourForUser:(KBUser *)user error:(NSError **)error;

- (KBKegTimeHost *)kegTimeHostWithName:(NSString *)name ipAddress:(NSString *)ipAddress port:(NSInteger)port;

+ (NSInteger)timeIndexForForDate:(NSDate *)date timeType:(KBPourIndexTimeType)timeType;

- (KBPourIndex *)pourIndexForDate:(NSDate *)date timeType:(KBPourIndexTimeType)timeType keg:(KBKeg *)keg user:(KBUser *)user error:(NSError **)error;

/*!
 Update pour index for keg and user for time index, and type.
 Will store 4 values for:
  - All kegs for all users
  - Keg for all users
  - User for all kegs
  - Keg and user
 */
- (BOOL)updatePourIndex:(float)amount date:(NSDate *)date timeType:(KBPourIndexTimeType)timeType keg:(KBKeg *)keg user:(KBUser *)user error:(NSError **)error;

- (NSArray */*of KBPourIndex*/)pourIndexesForStartIndex:(NSInteger)startIndex endIndex:(NSInteger)endIndex timeType:(KBPourIndexTimeType)timeType 
                                                    keg:(KBKeg *)keg user:(KBUser *)user error:(NSError **)error;

/*!
 Pour indexes for top volume poured by day.
 */
- (NSArray */*of KBPourIndex*/)topVolumePourIndexesWithOffset:(NSUInteger)offset limit:(NSUInteger)limit timeType:(KBPourIndexTimeType)timeType error:(NSError **)error;

@end
