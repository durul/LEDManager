/*
 * Copyright 2017 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <Foundation/Foundation.h>
#import "FIRDatabaseQuery.h"
#import "FIRDatabase.h"
#import "FIRDatabaseSwiftNameSupport.h"
#import "FIRDataSnapshot.h"
#import "FIRMutableData.h"
#import "FIRTransactionResult.h"
#import "FIRServerValue.h"

NS_ASSUME_NONNULL_BEGIN

@class FIRDatabase;

/**
 * A FIRDatabaseReference represents a particular location in your Firebase Database
 * and can be used for reading or writing data to that Firebase Database location.
 *
 * This class is the starting point for all Firebase Database operations. After you've
 * obtained your first FIRDatabaseReference via [FIRDatabase reference], you can use it
 * to read data (ie. observeEventType:withBlock:), write data (ie. setValue:), and to
 * create new FIRDatabaseReferences (ie. child:).
 */
FIR_SWIFT_NAME(DatabaseReference)
@interface FIRDatabaseReference : FIRDatabaseQuery


#pragma mark - Getting references to children locations

/**
 * Gets a FIRDatabaseReference for the location at the specified relative path.
 * The relative path can either be a simple child key (e.g. 'fred') or a
 * deeper slash-separated path (e.g. 'fred/name/first').
 *
 * @param pathString A relative path from this location to the desired child location.
 * @return A FIRDatabaseReference for the specified relative path.
 */
- (FIRDatabaseReference *)child:(NSString *)pathString;

/**
 * childByAppendingPath: is deprecated, use child: instead.
 */
- (FIRDatabaseReference *)childByAppendingPath:(NSString *)pathString __deprecated_msg("use child: instead");

/**
 * childByAutoId generates a new child location using a unique key and returns a
 * FIRDatabaseReference to it. This is useful when the children of a Firebase Database
 * location represent a list of items.
 *
 * The unique key generated by childByAutoId: is prefixed with a client-generated
 * timestamp so that the resulting list will be chronologically-sorted.
 *
 * @return A FIRDatabaseReference for the generated location.
 */
- (FIRDatabaseReference *) childByAutoId;


#pragma mark - Writing data

/** Write data to this Firebase Database location.

This will overwrite any data at this location and all child locations. 
 
Data types that can be set are:

- NSString -- @"Hello World"
- NSNumber (also includes boolean) -- @YES, @43, @4.333
- NSDictionary -- @{@"key": @"value", @"nested": @{@"another": @"value"} }
- NSArray

The effect of the write will be visible immediately and the corresponding
events will be triggered. Synchronization of the data to the Firebase Database
servers will also be started.
 
Passing null for the new value is equivalent to calling remove:;
all data at this location or any child location will be deleted.

Note that setValue: will remove any priority stored at this location, so if priority
is meant to be preserved, you should use setValue:andPriority: instead.

@param value The value to be written.
 */
- (void) setValue:(nullable id)value;


/**
 * The same as setValue: with a block that gets triggered after the write operation has
 * been committed to the Firebase Database servers.
 *
 * @param value The value to be written.
 * @param block The block to be called after the write has been committed to the Firebase Database servers.
 */
- (void) setValue:(nullable id)value withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


/**
 * The same as setValue: with an additional priority to be attached to the data being written.
 * Priorities are used to order items.
 *
 * @param value The value to be written.
 * @param priority The priority to be attached to that data.
 */
- (void) setValue:(nullable id)value andPriority:(nullable id)priority;


/**
 * The same as setValue:andPriority: with a block that gets triggered after the write operation has
 * been committed to the Firebase Database servers.
 *
 * @param value The value to be written.
 * @param priority The priority to be attached to that data.
 * @param block The block to be called after the write has been committed to the Firebase Database servers.
 */
- (void) setValue:(nullable id)value andPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


/**
 * Remove the data at this Firebase Database location. Any data at child locations will also be deleted.
 * 
 * The effect of the delete will be visible immediately and the corresponding events
 * will be triggered. Synchronization of the delete to the Firebase Database servers will
 * also be started.
 *
 * remove: is equivalent to calling setValue:nil
 */
- (void) removeValue;


/**
 * The same as remove: with a block that gets triggered after the remove operation has
 * been committed to the Firebase Database servers.
 *
 * @param block The block to be called after the remove has been committed to the Firebase Database servers.
 */
- (void) removeValueWithCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;

/**
 * Sets a priority for the data at this Firebase Database location.
 * Priorities can be used to provide a custom ordering for the children at a location
 * (if no priorities are specified, the children are ordered by key).
 *
 * You cannot set a priority on an empty location. For this reason
 * setValue:andPriority: should be used when setting initial data with a specific priority
 * and setPriority: should be used when updating the priority of existing data.
 *
 * Children are sorted based on this priority using the following rules:
 *
 * Children with no priority come first.
 * Children with a number as their priority come next. They are sorted numerically by priority (small to large).
 * Children with a string as their priority come last. They are sorted lexicographically by priority.
 * Whenever two children have the same priority (including no priority), they are sorted by key. Numeric
 * keys come first (sorted numerically), followed by the remaining keys (sorted lexicographically).
 * 
 * Note that priorities are parsed and ordered as IEEE 754 double-precision floating-point numbers.
 * Keys are always stored as strings and are treated as numbers only when they can be parsed as a
 * 32-bit integer
 *
 * @param priority The priority to set at the specified location.
 */
- (void) setPriority:(nullable id)priority;


/**
 * The same as setPriority: with a block that is called once the priority has
 * been committed to the Firebase Database servers.
 *
 * @param priority The priority to set at the specified location.
 * @param block The block that is triggered after the priority has been written on the servers.
 */
- (void) setPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;

/**
 * Updates the values at the specified paths in the dictionary without overwriting other
 * keys at this location.
 *
 * @param values A dictionary of the keys to change and their new values
 */
- (void) updateChildValues:(NSDictionary *)values;

/**
 * The same as update: with a block that is called once the update has been committed to the 
 * Firebase Database servers
 *
 * @param values A dictionary of the keys to change and their new values
 * @param block The block that is triggered after the update has been written on the Firebase Database servers
 */
- (void) updateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


#pragma mark - Attaching observers to read data

/**
 * observeEventType:withBlock: is used to listen for data changes at a particular location.
 * This is the primary way to read data from the Firebase Database. Your block will be triggered
 * for the initial data and again whenever the data changes.
 *
 * Use removeObserverWithHandle: to stop receiving updates.
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.  It is passed the data as a FIRDataSnapshot.
 * @return A handle used to unregister this block later using removeObserverWithHandle:
 */
- (FIRDatabaseHandle)observeEventType:(FIRDataEventType)eventType withBlock:(void (^)(FIRDataSnapshot *snapshot))block;


/**
 * observeEventType:andPreviousSiblingKeyWithBlock: is used to listen for data changes at a particular location.
 * This is the primary way to read data from the Firebase Database. Your block will be triggered
 * for the initial data and again whenever the data changes. In addition, for FIRDataEventTypeChildAdded, FIRDataEventTypeChildMoved, and
 * FIRDataEventTypeChildChanged events, your block will be passed the key of the previous node by priority order.
 *
 * Use removeObserverWithHandle: to stop receiving updates.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.  It is passed the data as a FIRDataSnapshot
 * and the previous child's key.
 * @return A handle used to unregister this block later using removeObserverWithHandle:
 */
- (FIRDatabaseHandle)observeEventType:(FIRDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(FIRDataSnapshot *snapshot, NSString *__nullable prevKey))block;


/**
 * observeEventType:withBlock: is used to listen for data changes at a particular location.
 * This is the primary way to read data from the Firebase Database. Your block will be triggered
 * for the initial data and again whenever the data changes.
 *
 * The cancelBlock will be called if you will no longer receive new events due to no longer having permission.
 *
 * Use removeObserverWithHandle: to stop receiving updates.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.  It is passed the data as a FIRDataSnapshot.
 * @param cancelBlock The block that should be called if this client no longer has permission to receive these events
 * @return A handle used to unregister this block later using removeObserverWithHandle:
 */
- (FIRDatabaseHandle)observeEventType:(FIRDataEventType)eventType withBlock:(void (^)(FIRDataSnapshot *snapshot))block withCancelBlock:(nullable void (^)(NSError* error))cancelBlock;


/**
 * observeEventType:andPreviousSiblingKeyWithBlock: is used to listen for data changes at a particular location.
 * This is the primary way to read data from the Firebase Database. Your block will be triggered
 * for the initial data and again whenever the data changes. In addition, for FIRDataEventTypeChildAdded, FIRDataEventTypeChildMoved, and
 * FIRDataEventTypeChildChanged events, your block will be passed the key of the previous node by priority order.
 *
 * The cancelBlock will be called if you will no longer receive new events due to no longer having permission.
 *
 * Use removeObserverWithHandle: to stop receiving updates.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.  It is passed the data as a FIRDataSnapshot
 * and the previous child's key.
 * @param cancelBlock The block that should be called if this client no longer has permission to receive these events
 * @return A handle used to unregister this block later using removeObserverWithHandle:
 */
- (FIRDatabaseHandle)observeEventType:(FIRDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(FIRDataSnapshot *snapshot, NSString *__nullable prevKey))block withCancelBlock:(nullable void (^)(NSError* error))cancelBlock;


/**
 * This is equivalent to observeEventType:withBlock:, except the block is immediately canceled after the initial data is returned.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called.  It is passed the data as a FIRDataSnapshot.
 */
- (void)observeSingleEventOfType:(FIRDataEventType)eventType withBlock:(void (^)(FIRDataSnapshot *snapshot))block;


/**
 * This is equivalent to observeEventType:withBlock:, except the block is immediately canceled after the initial data is returned. In addition, for FIRDataEventTypeChildAdded, FIRDataEventTypeChildMoved, and
 * FIRDataEventTypeChildChanged events, your block will be passed the key of the previous node by priority order.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called.  It is passed the data as a FIRDataSnapshot and the previous child's key.
 */
- (void)observeSingleEventOfType:(FIRDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(FIRDataSnapshot *snapshot, NSString *__nullable prevKey))block;


/**
 * This is equivalent to observeEventType:withBlock:, except the block is immediately canceled after the initial data is returned.
 *
 * The cancelBlock will be called if you do not have permission to read data at this location.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called.  It is passed the data as a FIRDataSnapshot.
 * @param cancelBlock The block that will be called if you don't have permission to access this data
 */
- (void)observeSingleEventOfType:(FIRDataEventType)eventType withBlock:(void (^)(FIRDataSnapshot *snapshot))block withCancelBlock:(nullable void (^)(NSError* error))cancelBlock;


/**
 * This is equivalent to observeEventType:withBlock:, except the block is immediately canceled after the initial data is returned. In addition, for FIRDataEventTypeChildAdded, FIRDataEventTypeChildMoved, and
 * FIRDataEventTypeChildChanged events, your block will be passed the key of the previous node by priority order.
 *
 * The cancelBlock will be called if you do not have permission to read data at this location.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called.  It is passed the data as a FIRDataSnapshot and the previous child's key.
 * @param cancelBlock The block that will be called if you don't have permission to access this data
 */
- (void)observeSingleEventOfType:(FIRDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(FIRDataSnapshot *snapshot, NSString *__nullable prevKey))block withCancelBlock:(nullable void (^)(NSError* error))cancelBlock;

#pragma mark - Detaching observers

/**
 * Detach a block previously attached with observeEventType:withBlock:.
 *
 * @param handle The handle returned by the call to observeEventType:withBlock: which we are trying to remove.
 */
- (void) removeObserverWithHandle:(FIRDatabaseHandle)handle;

/**
 * By calling `keepSynced:YES` on a location, the data for that location will automatically be downloaded and
 * kept in sync, even when no listeners are attached for that location. Additionally, while a location is kept
 * synced, it will not be evicted from the persistent disk cache.
 *
 * @param keepSynced Pass YES to keep this location synchronized, pass NO to stop synchronization.
 */
- (void) keepSynced:(BOOL)keepSynced;


/**
 * Removes all observers at the current reference, but does not remove any observers at child references.
 * removeAllObservers must be called again for each child reference where a listener was established to remove the observers.
 */
- (void) removeAllObservers;

#pragma mark - Querying and limiting


/**
 * queryLimitedToFirst: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryLimitedToFirst: will respond to at most the first limit child nodes.
 *
 * @param limit The upper bound, inclusive, for the number of child nodes to receive events for
 * @return A FIRDatabaseQuery instance, limited to at most limit child nodes.
 */
- (FIRDatabaseQuery *)queryLimitedToFirst:(NSUInteger)limit;


/**
 * queryLimitedToLast: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryLimitedToLast: will respond to at most the last limit child nodes.
 *
 * @param limit The upper bound, inclusive, for the number of child nodes to receive events for
 * @return A FIRDatabaseQuery instance, limited to at most limit child nodes.
 */
- (FIRDatabaseQuery *)queryLimitedToLast:(NSUInteger)limit;

/**
 * queryOrderBy: is used to generate a reference to a view of the data that's been sorted by the values of
 * a particular child key. This method is intended to be used in combination with queryStartingAtValue:,
 * queryEndingAtValue:, or queryEqualToValue:.
 *
 * @param key The child key to use in ordering data visible to the returned FIRDatabaseQuery
 * @return A FIRDatabaseQuery instance, ordered by the values of the specified child key.
 */
- (FIRDatabaseQuery *)queryOrderedByChild:(NSString *)key;

/**
 * queryOrderedByKey: is used to generate a reference to a view of the data that's been sorted by child key.
 * This method is intended to be used in combination with queryStartingAtValue:, queryEndingAtValue:,
 * or queryEqualToValue:.
 *
 * @return A FIRDatabaseQuery instance, ordered by child keys.
 */
- (FIRDatabaseQuery *) queryOrderedByKey;

/**
 * queryOrderedByPriority: is used to generate a reference to a view of the data that's been sorted by child
 * priority. This method is intended to be used in combination with queryStartingAtValue:, queryEndingAtValue:,
 * or queryEqualToValue:.
 *
 * @return A FIRDatabaseQuery instance, ordered by child priorities.
 */
- (FIRDatabaseQuery *) queryOrderedByPriority;

/**
 * queryStartingAtValue: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryStartingAtValue: will respond to events at nodes with a value
 * greater than or equal to startValue.
 *
 * @param startValue The lower bound, inclusive, for the value of data visible to the returned FIRDatabaseQuery
 * @return A FIRDatabaseQuery instance, limited to data with value greater than or equal to startValue
 */
- (FIRDatabaseQuery *)queryStartingAtValue:(nullable id)startValue;

/**
 * queryStartingAtValue:childKey: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryStartingAtValue:childKey will respond to events at nodes with a value
 * greater than startValue, or equal to startValue and with a key greater than or equal to childKey.
 *
 * @param startValue The lower bound, inclusive, for the value of data visible to the returned FIRDatabaseQuery
 * @param childKey The lower bound, inclusive, for the key of nodes with value equal to startValue
 * @return A FIRDatabaseQuery instance, limited to data with value greater than or equal to startValue
 */
- (FIRDatabaseQuery *)queryStartingAtValue:(nullable id)startValue childKey:(nullable NSString *)childKey;

/**
 * queryEndingAtValue: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryEndingAtValue: will respond to events at nodes with a value
 * less than or equal to endValue.
 *
 * @param endValue The upper bound, inclusive, for the value of data visible to the returned FIRDatabaseQuery
 * @return A FIRDatabaseQuery instance, limited to data with value less than or equal to endValue
 */
- (FIRDatabaseQuery *)queryEndingAtValue:(nullable id)endValue;

/**
 * queryEndingAtValue:childKey: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryEndingAtValue:childKey will respond to events at nodes with a value
 * less than endValue, or equal to endValue and with a key less than or equal to childKey.
 *
 * @param endValue The upper bound, inclusive, for the value of data visible to the returned FIRDatabaseQuery
 * @param childKey The upper bound, inclusive, for the key of nodes with value equal to endValue
 * @return A FIRDatabaseQuery instance, limited to data with value less than or equal to endValue
 */
- (FIRDatabaseQuery *)queryEndingAtValue:(nullable id)endValue childKey:(nullable NSString *)childKey;

/**
 * queryEqualToValue: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryEqualToValue: will respond to events at nodes with a value equal
 * to the supplied argument.
 *
 * @param value The value that the data returned by this FIRDatabaseQuery will have
 * @return A FIRDatabaseQuery instance, limited to data with the supplied value.
 */
- (FIRDatabaseQuery *)queryEqualToValue:(nullable id)value;

/**
 * queryEqualToValue:childKey: is used to generate a reference to a limited view of the data at this location.
 * The FIRDatabaseQuery instance returned by queryEqualToValue:childKey will respond to events at nodes with a value
 * equal to the supplied argument with a key equal to childKey. There will be at most one node that matches because
 * child keys are unique.
 *
 * @param value The value that the data returned by this FIRDatabaseQuery will have
 * @param childKey The key of nodes with the right value
 * @return A FIRDatabaseQuery instance, limited to data with the supplied value and the key.
 */
- (FIRDatabaseQuery *)queryEqualToValue:(nullable id)value childKey:(nullable NSString *)childKey;

#pragma mark - Managing presence

/**
 * Ensure the data at this location is set to the specified value when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 * onDisconnectSetValue: is especially useful for implementing "presence" systems,
 * where a value should be changed or cleared when a user disconnects
 * so that he appears "offline" to other users.
 *
 * @param value The value to be set after the connection is lost.
 */
- (void) onDisconnectSetValue:(nullable id)value;


/**
 * Ensure the data at this location is set to the specified value when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 * The completion block will be triggered when the operation has been successfully queued up on the Firebase Database servers
 *
 * @param value The value to be set after the connection is lost.
 * @param block Block to be triggered when the operation has been queued up on the Firebase Database servers
 */
- (void) onDisconnectSetValue:(nullable id)value withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


/**
 * Ensure the data at this location is set to the specified value and priority when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 * @param value The value to be set after the connection is lost.
 * @param priority The priority to be set after the connection is lost.
 */
- (void) onDisconnectSetValue:(nullable id)value andPriority:(id)priority;


/**
 * Ensure the data at this location is set to the specified value and priority when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 * The completion block will be triggered when the operation has been successfully queued up on the Firebase Database servers
 *
 * @param value The value to be set after the connection is lost.
 * @param priority The priority to be set after the connection is lost.
 * @param block Block to be triggered when the operation has been queued up on the Firebase Database servers
 */
- (void) onDisconnectSetValue:(nullable id)value andPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


/**
 * Ensure the data at this location is removed when
 * the client is disconnected (due to closing the app, navigating
 * to a new page, or network issues).
 *
 * onDisconnectRemoveValue is especially useful for implementing "presence" systems.
 */
- (void) onDisconnectRemoveValue;


/**
 * Ensure the data at this location is removed when
 * the client is disconnected (due to closing the app, navigating
 * to a new page, or network issues).
 *
 * onDisconnectRemoveValueWithCompletionBlock: is especially useful for implementing "presence" systems.
 *
 * @param block Block to be triggered when the operation has been queued up on the Firebase Database servers
 */
- (void) onDisconnectRemoveValueWithCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;



/**
 * Ensure the data has the specified child values updated when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 *
 * @param values A dictionary of child node keys and the values to set them to after the connection is lost.
 */
- (void) onDisconnectUpdateChildValues:(NSDictionary *)values;


/**
 * Ensure the data has the specified child values updated when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 *
 * @param values A dictionary of child node keys and the values to set them to after the connection is lost.
 * @param block A block that will be called once the operation has been queued up on the Firebase Database servers
 */
- (void) onDisconnectUpdateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


/**
 * Cancel any operations that are set to run on disconnect. If you previously called onDisconnectSetValue:,
 * onDisconnectRemoveValue:, or onDisconnectUpdateChildValues:, and no longer want the values updated when the 
 * connection is lost, call cancelDisconnectOperations:
 */
- (void) cancelDisconnectOperations;


/**
 * Cancel any operations that are set to run on disconnect. If you previously called onDisconnectSetValue:,
 * onDisconnectRemoveValue:, or onDisconnectUpdateChildValues:, and no longer want the values updated when the
 * connection is lost, call cancelDisconnectOperations:
 *
 * @param block A block that will be triggered once the Firebase Database servers have acknowledged the cancel request.
 */
- (void) cancelDisconnectOperationsWithCompletionBlock:(nullable void (^)(NSError *__nullable error, FIRDatabaseReference * ref))block;


#pragma mark - Manual Connection Management

/**
 * Manually disconnect the Firebase Database client from the server and disable automatic reconnection.
 *
 * The Firebase Database client automatically maintains a persistent connection to the Firebase Database server,
 * which will remain active indefinitely and reconnect when disconnected. However, the goOffline( ) 
 * and goOnline( ) methods may be used to manually control the client connection in cases where 
 * a persistent connection is undesirable.
 * 
 * While offline, the Firebase Database client will no longer receive data updates from the server. However,
 * all database operations performed locally will continue to immediately fire events, allowing
 * your application to continue behaving normally. Additionally, each operation performed locally 
 * will automatically be queued and retried upon reconnection to the Firebase Database server.
 * 
 * To reconnect to the Firebase Database server and begin receiving remote events, see goOnline( ).
 * Once the connection is reestablished, the Firebase Database client will transmit the appropriate data
 * and fire the appropriate events so that your client "catches up" automatically.
 * 
 * Note: Invoking this method will impact all Firebase Database connections.
 */
+ (void) goOffline;

/**
 * Manually reestablish a connection to the Firebase Database server and enable automatic reconnection.
 *
 * The Firebase Database client automatically maintains a persistent connection to the Firebase Database server,
 * which will remain active indefinitely and reconnect when disconnected. However, the goOffline( ) 
 * and goOnline( ) methods may be used to manually control the client connection in cases where 
 * a persistent connection is undesirable.
 * 
 * This method should be used after invoking goOffline( ) to disable the active connection. 
 * Once reconnected, the Firebase Database client will automatically transmit the proper data and fire
 * the appropriate events so that your client "catches up" automatically.
 * 
 * To disconnect from the Firebase Database server, see goOffline( ).
 * 
 * Note: Invoking this method will impact all Firebase Database connections.
 */
+ (void) goOnline;


#pragma mark - Transactions

/**
 * Performs an optimistic-concurrency transactional update to the data at this location. Your block will be called with a FIRMutableData
 * instance that contains the current data at this location. Your block should update this data to the value you
 * wish to write to this location, and then return an instance of FIRTransactionResult with the new data.
 *
 * If, when the operation reaches the server, it turns out that this client had stale data, your block will be run
 * again with the latest data from the server.
 *
 * When your block is run, you may decide to abort the transaction by returning [FIRTransactionResult abort].
 *
 * @param block This block receives the current data at this location and must return an instance of FIRTransactionResult
 */
- (void) runTransactionBlock:(FIRTransactionResult * (^) (FIRMutableData* currentData))block;


/**
 * Performs an optimistic-concurrency transactional update to the data at this location. Your block will be called with a FIRMutableData
 * instance that contains the current data at this location. Your block should update this data to the value you
 * wish to write to this location, and then return an instance of FIRTransactionResult with the new data.
 *
 * If, when the operation reaches the server, it turns out that this client had stale data, your block will be run
 * again with the latest data from the server.
 *
 * When your block is run, you may decide to abort the transaction by returning [FIRTransactionResult abort].
 *
 * @param block This block receives the current data at this location and must return an instance of FIRTransactionResult
 * @param completionBlock This block will be triggered once the transaction is complete, whether it was successful or not. It will indicate if there was an error, whether or not the data was committed, and what the current value of the data at this location is.
 */
- (void)runTransactionBlock:(FIRTransactionResult * (^) (FIRMutableData* currentData))block andCompletionBlock:(void (^) (NSError *__nullable error, BOOL committed, FIRDataSnapshot *__nullable snapshot))completionBlock;



/**
 * Performs an optimistic-concurrency transactional update to the data at this location. Your block will be called with a FIRMutableData
 * instance that contains the current data at this location. Your block should update this data to the value you
 * wish to write to this location, and then return an instance of FIRTransactionResult with the new data.
 *
 * If, when the operation reaches the server, it turns out that this client had stale data, your block will be run
 * again with the latest data from the server.
 *
 * When your block is run, you may decide to abort the transaction by return [FIRTransactionResult abort].
 *
 * Since your block may be run multiple times, this client could see several immediate states that don't exist on the server. You can suppress those immediate states until the server confirms the final state of the transaction.
 *
 * @param block This block receives the current data at this location and must return an instance of FIRTransactionResult
 * @param completionBlock This block will be triggered once the transaction is complete, whether it was successful or not. It will indicate if there was an error, whether or not the data was committed, and what the current value of the data at this location is.
 * @param localEvents Set this to NO to suppress events raised for intermediate states, and only get events based on the final state of the transaction.
 */
- (void)runTransactionBlock:(FIRTransactionResult * (^) (FIRMutableData* currentData))block andCompletionBlock:(nullable void (^) (NSError *__nullable error, BOOL committed, FIRDataSnapshot *__nullable snapshot))completionBlock withLocalEvents:(BOOL)localEvents;


#pragma mark - Retrieving String Representation

/**
 * Gets the absolute URL of this Firebase Database location.
 *
 * @return The absolute URL of the referenced Firebase Database location.
 */
- (NSString *) description;

#pragma mark - Properties

/**
 * Gets a FIRDatabaseReference for the parent location.
 * If this instance refers to the root of your Firebase Database, it has no parent,
 * and therefore parent( ) will return null.
 *
 * @return A FIRDatabaseReference for the parent location.
 */
@property (strong, readonly, nonatomic, nullable) FIRDatabaseReference * parent;


/**
 * Gets a FIRDatabaseReference for the root location
 *
 * @return A new FIRDatabaseReference to root location.
 */
@property (strong, readonly, nonatomic) FIRDatabaseReference * root;


/**
 * Gets the last token in a Firebase Database location (e.g. 'fred' in https&#58;//SampleChat.firebaseIO-demo.com/users/fred)
 *
 * @return The key of the location this reference points to.
 */
@property (strong, readonly, nonatomic) NSString* key;

/**
 * Gets the URL for the Firebase Database location referenced by this FIRDatabaseReference.
 *
 * @return The url of the location this reference points to.
 */
@property (strong, readonly, nonatomic) NSString* URL;

/**
 * Gets the FIRDatabase instance associated with this reference.
 *
 * @return The FIRDatabase object for this reference.
 */
@property (strong, readonly, nonatomic) FIRDatabase *database;

@end

NS_ASSUME_NONNULL_END
