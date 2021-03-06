//
//  TGMapView.h
//  TangramMap
//
//  Created by Matt Blair on 7/9/18.
//

#import "TGExport.h"
#import "TGGeoPoint.h"
#import "TGMapData.h"
#import "TGTypes.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class TGHttpHandler;
@class TGLabelPickResult;
@class TGMapData;
@class TGMarker;
@class TGMarkerPickResult;
@class TGSceneUpdate;

@protocol TGMapViewDelegate;
@protocol TGRecognizerDelegate;

/**
 `TGMapView` is a flexible and customizable map view managing the lifecycle of an OpenGL ES map. This view provides
 gesture handlers for tap, double-tap, long press, pan, pinch, rotate, and shove gestures.

 The public interface provides dynamic map marker placement, change of camera view settings, and map description changes
 through scene updates.

 This view uses scenes descibed by the <a href="https://mapzen.com/documentation/tangram/">Tangram scene format</a>
 allowing you to fully customize your map using your own data.
 Some pre-made basemap styles can be found <a href="https://mapzen.com/documentation/cartography/styles/">here</a> using
 Mapzen data sources.

 To use basemap styles you can <a href="https://mapzen.com/developers/sign_in"> sign up for an API key</a> and load it
 through your application:

 ```swift
 let sceneURL = URL("https://mapzen.com/carto/walkabout-style-more-labels/walkabout-style-more-labels.yaml");
 let sceneUpdates = [ TGSceneUpdate(path: "sources.mapzen.url_params", value: "{ api_key: \(YOUR_API_KEY) }") ];
 view.loadScene(from: sceneURL, with: sceneUpdates);
 ```
 @note All the screen positions used in this inteface are in _logical pixel_ or _drawing coordinate system_ (based on a
 `UIKit` coordinate system), which is independent of the phone pixel density. Refer to the
 <a href="https://developer.apple.com/library/content/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/GraphicsDrawingOverview/GraphicsDrawingOverview.html">Apple documentation</a>
 regarding _Coordinate Systems and Drawing in iOS_ for more information.
 */
TG_EXPORT
@interface TGMapView : UIView

#pragma mark Loading Scenes

/**
 Load a scene file synchronously from a URL with a list of updates.

 If an error occurs while applying updates the new scene will not be applied. See `TGSceneUpdate` for details.

 @param url An http(s) or file URL for the scene file.
 @param updates A list of `TGSceneUpdate` to apply to the scene, or `nil`.
 @return The integer ID for the new scene or -1 if the scene cannot be loaded.
 */
- (int)loadSceneFromURL:(NSURL *)url withUpdates:(nullable NSArray<TGSceneUpdate *> *)updates;

/**
 Load a scene file asynchronously from a URL with a list of updates.

 Calls `-[TGMapViewDelegate mapView:didLoadScene:withError:]` on the `mapViewDelegate` when it completes.

 If an error occurs while applying updates the new scene will not be applied. See `TGSceneUpdate` for details.

 @param url An http(s) or file URL for the scene file.
 @param updates A list of `TGSceneUpdate` to apply to the scene, or `nil`.
 @return The integer ID for the new scene or -1 if the scene cannot be loaded.
 */
- (int)loadSceneAsyncFromURL:(NSURL *)url withUpdates:(nullable NSArray<TGSceneUpdate *> *)updates;

/**
 Load a scene synchronously from string with a list of updates.

 Calls `-[TGMapViewDelegate mapView:didLoadScene:withError:]` on the `mapViewDelegate` when it completes.

 If an error occurs while applying updates the new scene will not be applied. See `TGSceneUpdate` for details.

 @param yaml YAML scene string.
 @param url The base URL used to resolve relative URLs in the scene.
 @param updates A list of `TGSceneUpdate` to apply to the scene, or `nil`.
 @return The integer ID for the new scene or -1 if the scene cannot be loaded.
 */
- (int)loadSceneFromYAML:(NSString *)yaml
           relativeToURL:(NSURL *)url
             withUpdates:(nullable NSArray<TGSceneUpdate *> *)updates;

/**
 Load a scene asynchronously from string with a list of updates.

 Calls `-[TGMapViewDelegate mapView:didLoadScene:withError:]` on the `mapViewDelegate` when it completes.

 If an error occurs while applying updates the new scene will not be applied. See `TGSceneUpdate` for details.

 @param yaml YAML scene string.
 @param url The base URL used to resolve relative URLs in the scene.
 @param updates A list of `TGSceneUpdate` to apply to the scene, or `nil`.
 @return The integer ID for the new scene or -1 if the scene cannot be loaded.
 */
- (int)loadSceneAsyncFromYAML:(NSString *)yaml
                relativeToURL:(NSURL *)url
                  withUpdates:(nullable NSArray<TGSceneUpdate *> *)updates;

/**
 Modify the current scene asynchronously with a list of updates.

 Calls `-[TGMapViewDelegate mapView:didLoadScene:withError:]` on the `mapViewDelegate` when it completes.

 If an error occurs while applying updates, no changes will be applied. See `TGSceneUpdate` for details.

 @param updates A list of `TGSceneUpdate` to apply to the scene.
 @return The integer ID for the updated scene or -1 if the scene cannot be updated.
 */
- (int)updateSceneAsync:(NSArray<TGSceneUpdate *> *)updates;

#pragma mark Delegates

/**
 The gesture recognizer delegate
 May be `nil`. See `TGRecognizerDelegate` for more details.
 */
@property (weak, nonatomic, nullable) id<TGRecognizerDelegate> gestureDelegate;

/**
 The map view delegate
 May be `nil`. See `TGMapViewDelegate` for more details.
 */
@property (weak, nonatomic, nullable) id<TGMapViewDelegate> mapViewDelegate;

#pragma mark Camera Properties

/**
 Assign a `TGCameraType` to the view camera
 */
@property (assign, nonatomic) TGCameraType cameraType;

/**
 Assign a longitude and latitude to the map view camera
 */
@property (assign, nonatomic) TGGeoPoint position;

/**
 Assign a floating point zoom to the map view camera
 */
@property (assign, nonatomic) float zoom;

/**
 Assign a rotation angle in radians to the map view camera
 */
@property (assign, nonatomic) float rotation;

/**
 Assign a tilt angle in radians to the map view camera
 */
@property (assign, nonatomic) float tilt;

#pragma mark Camera Animation

/**
 Animate the map view to a center coordinate.

 @param position The center coordinate.
 @param seconds The animation duration in seconds.

 @note The default ease type for this animation is cubic, see `TGEaseType` for details.
 */
- (void)animateToPosition:(TGGeoPoint)position withDuration:(float)seconds;

/**
 Animate the map view to a center coordinate with an easing function.

 @param position The center coordinate.
 @param seconds The animation duration in seconds.
 @param easeType The ease type to use. See `TGEaseType` for more details.
 */
- (void)animateToPosition:(TGGeoPoint)position withDuration:(float)seconds withEaseType:(TGEaseType)easeType;

/**
 Animate the map view to a zoom level.

 @param zoomLevel The zoom level.
 @param seconds The animation duration in seconds.

 @note The default ease type for this animation is cubic, see `TGEaseType` for details.
 */
- (void)animateToZoomLevel:(float)zoomLevel withDuration:(float)seconds;

/**
 Animate the map view to a zoom level with an easing function.

 @param zoomLevel The zoom level.
 @param seconds The animation duration in seconds.
 @param easeType The ease type to use. See `TGEaseType` for more details.
 */
- (void)animateToZoomLevel:(float)zoomLevel withDuration:(float)seconds withEaseType:(TGEaseType)easeType;

/**
 Animate the map view to a rotation.

 @param radians The rotation in radians.
 @param seconds The animation duration in seconds.

 @note The default ease type for this animation is cubic, see `TGEaseType` for details.
 */
- (void)animateToRotation:(float)radians withDuration:(float)seconds;

/**
 Animate the map view to a rotation with an easing function.

 @param radians The rotation in radians.
 @param seconds The animation duration in seconds.
 @param easeType The ease type to use. See `TGEaseType` for more details.
 */
- (void)animateToRotation:(float)radians withDuration:(float)seconds withEaseType:(TGEaseType)easeType;

/**
 Animate the map view to a tilt angle.

 @param radians The tilt angle in radians.
 @param seconds The animation duration in seconds.

 @note The default ease type for this animation is cubic, see `TGEaseType` for details.
 */
- (void)animateToTilt:(float)radians withDuration:(float)seconds;

/**
 Animate the map view to a tilt angle with an easing function.

 @param radians The tilt angle in radians.
 @param seconds The animation duration in seconds.
 @param easeType The ease type to use, see `TGEaseType` for more details.
 */
- (void)animateToTilt:(float)radians withDuration:(float)seconds withEaseType:(TGEaseType)easeType;

#pragma mark Coordinate Conversions

/**
 Convert a longitude and latitude to a view position.

 @param lngLat The geographic coordinate to convert.
 @return The view position of the input coordinate, or `(NAN, NAN)` if the coordinate is not visible in the view.
 */
- (CGPoint)lngLatToScreenPosition:(TGGeoPoint)lngLat;

/**
 Given coordinates in screen space (`x` right, `y` down), set the output longitude and latitude to the geographic
 location corresponding to that point.

 @param viewPosition the 2d screen position to convert
 @return the longitude and latitude, or `(NAN, NAN)` if the point is not visible on the screen.
 */
- (TGGeoPoint)screenPositionToLngLat:(CGPoint)viewPosition;


#pragma mark Markers

/**
 Remove all the Markers from the map.
 */
- (void)markerRemoveAll;

/**
 Create a marker and add it to the map.

 @return The new Marker.

 @note The new Marker will not be usable until you set its styling, geometry, or image depending on your use case.
 */
- (TGMarker*)markerAdd;

/**
 Remove a Marker from the map.

 @param marker The Marker to remove.

 @note Do not use a Marker after removing it from the map.
 */
- (void)markerRemove:(TGMarker*)marker;

/**
 Access the Markers added to the map.
 */
@property (readonly, nonatomic) NSArray<TGMarker *>* markers;

#pragma mark File Handling

/**
 Assign a `TGHttpHandler` for network request management.

 A default handler will be used if this is not set.

 @note Assigning the http handler is optional and should only be done if you want to change any network access behavior
 (e.g. specify cache location and size or be notified when a network request completes). See `TGHTTPHandler` for more
 information on the default provided configuration.
 */
@property (strong, nonatomic) TGHttpHandler* httpHandler;

/**
 Assign the resource root for this map view.

 Scene file URLs will be resolved relative to this URL.

 @note By default the resource root is the main bundle resource URL. Using the default resource root: `scene.yaml` is
 resolved to `file://<main bundle path>/Resources/scene.yaml`, `/path/scene.yaml` is resolved to
 `file:///path/scene.yaml`, and `https://my.host/scene.yaml` is resolved to itself.
 */
@property (strong, nonatomic) NSURL* resourceRoot;

#pragma mark Rendering Behavior

/**
 Request the view to draw another frame.

 Typically there is no need to call this. The map view re-draws automatically when needed.
 */
- (void)requestRender;

/**
 The rate you want the map view to re-draw its contents.

 The default value is 60 frames per second.
 */
@property (assign, nonatomic) NSInteger preferredFramesPerSecond;

/**
 If `continuous` is `YES`, the map view will re-draw continuously. Otherwise, the map will re-draw only when an event
 changes the map view.

 Scenes can be configured as `animated` (see the <a href="https://mapzen.com/documentation/tangram/scene/#animated">
 animated property documentation</a> for details). When a scene is loaded this property is set to match the animated
 value from the scene.

 @note Changing this property will override the inferred value from the scene. Enabling continuous rendering can
 significantly increase the energy usage of an application.
 */
@property (assign, nonatomic) BOOL continuous;

#pragma mark Gesture Recognizers

/**
 Replaces the tap gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UITapGestureRecognizer* tapGestureRecognizer;

/**
 Replaces the double tap gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UITapGestureRecognizer* doubleTapGestureRecognizer;

/**
 Replaces the pan gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UIPanGestureRecognizer* panGestureRecognizer;

/**
 Replaces the pinch gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UIPinchGestureRecognizer* pinchGestureRecognizer;

/**
 Replaces the rotation gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UIRotationGestureRecognizer* rotationGestureRecognizer;

/**
 Replaces the shove gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UIPanGestureRecognizer* shoveGestureRecognizer;

/**
 Replaces the long press gesture recognizer used by the map view and adds it to the UIView.
 */
@property (strong, nonatomic) UILongPressGestureRecognizer* longPressGestureRecognizer;

#pragma mark Data Layers

/**
 Adds a named data layer to the map.

 @param name The name of the data layer.
 @param generateCentroid If YES, a point feature will be added at the centroid of every polygon feature. This can be
 useful for labeling.
 @return The map data, or `nil` if the data source can't be initialized.

 @note You cannot create more than one data source with the same name. If you call this with a name that is already in
 use, the previously returned object will be returned again.
 */
- (nullable TGMapData *)addDataLayer:(NSString *)name generateCentroid:(BOOL)generateCentroid;

#pragma mark Screenshots

/**
 Capture a screenshot of the map view.

 The captured screenshot will be delivered to the `mapViewDelegate` by the `mapView:didCaptureScreenshot:` method. The
 delegate must implement this method to receive the screenshot.

 @param waitForViewComplete If YES, the view will wait for all parts of the map in the current view to finish loading
 before taking the screenshot.
 */
- (void)captureScreenshot:(BOOL)waitForViewComplete;

#pragma mark Picking Map Objects

/**
 Set the radius in logical pixels to use when picking features on the map (default is `0.5`).

 The `-pick*` methods will retrieve all `interactive` map objects from a circular area with this radius around the pick
 location. Setting a larger radius can help ensure that desired features are retrieved from an imprecise touch input.

 @param pixels The pick radius in logical pixels.
 */
- (void)setPickRadius:(float)pixels;

/**
 Select a visible feature marked as `interactive` from the map view.

 The pick result will be delivered to the `mapViewDelegate` by `mapView:didSelectFeature:atScreenPosition:`.

 The scene file determines which features can be picked. See the
 <a href="https://mapzen.com/documentation/tangram/draw/#interactive">interactive property documentation</a> for
 details.

 @param viewPosition The position in the view to pick from, in logical pixels.
 */
- (void)pickFeatureAt:(CGPoint)viewPosition;

/**
 Select a label marked as `interactive` from the map view.

 The pick result will be delivered to the `mapViewDelegate` by `mapView:didSelectFeature:atScreenPosition:`.

 The scene file determines which features can be picked. See the
 <a href="https://mapzen.com/documentation/tangram/draw/#interactive"> interactive property documentation</a> for
 details.

 @param viewPosition The position in the view to pick from, in logical pixels.
 */
- (void)pickLabelAt:(CGPoint)viewPosition;

/**
 Select a Marker marked as `interactive` from the map view.

 The pick result will be delivered to the `mapViewDelegate` by `mapView:didSelectFeature:atScreenPosition:`.

 To pick a marker you must set the `interactive` property when styling it:
 ```swift
 TGMarker marker;
 marker.styling = "{ style: 'points', interactive : true,  color: 'white', size: [30px, 30px], order: 500 }"
 ```

 @param viewPosition The position in the view to pick from, in logical pixels.
 */
- (void)pickMarkerAt:(CGPoint)viewPosition;

#pragma mark Memory Management

/**
 Reduce memory usage by freeing currently unused resources.
 */
- (void)didReceiveMemoryWarning;

#pragma mark Debugging

/**
 Set a `TGDebugFlag` on the map view.

 @param debugFlag The debug flag to set.
 @param on Whether the flag is on or off.
 */
- (void)setDebugFlag:(TGDebugFlag)debugFlag value:(BOOL)on;

/**
 Get the status of a `TGDebugFlag`.

 @param debugFlag The debug flag to get the status of.
 @return Whether the flag is currently on or off for the map view.
 */
- (BOOL)getDebugFlag:(TGDebugFlag)debugFlag;

/**
 Invert the state of a `TGDebugFlag`.

 @param debugFlag The debug flag to toggle the state of.
 */
- (void)toggleDebugFlag:(TGDebugFlag)debugFlag;

@end // interface TGMapView

NS_ASSUME_NONNULL_END

/**
 A map view delegate can receive various map events.

 @note All of these methods are called from main thread, these methods are all **optional**.
 */
@protocol TGMapViewDelegate <NSObject>
@optional

/**
 Receive the result from `-[TGMapView pickFeatureAt:]`.

 @param mapView The map view instance.
 @param feature A dictionary of properties of the picked feature or `nil` if no feature was found.
 @param position The view position where the feature was picked.
 */
- (void)mapView:(nonnull TGMapView *)mapView
didSelectFeature:(nullable TGFeatureProperties *)feature
atScreenPosition:(CGPoint)position;

/**
 Receive the result from `-[TGMapView pickLabelAt:]`.

 @param mapView The map view instance.
 @param labelPickResult A result object with information about the picked label or `nil` if no label was found.
 @param position The view position where the label was picked.
 */
- (void)mapView:(nonnull TGMapView *)mapView
 didSelectLabel:(nullable TGLabelPickResult *)labelPickResult
atScreenPosition:(CGPoint)position;

/**
 Receive the result from `-[TGMapView pickMarkerAt:]`.

 @param mapView The map view instance.
 @param markerPickResult A result object with information about the picked marker or `nil` if no marker was found.
 @param position The view position where the marker was picked.
 */
- (void)mapView:(nonnull TGMapView *)mapView
didSelectMarker:(nullable TGMarkerPickResult *)markerPickResult
atScreenPosition:(CGPoint)position;

/**
 Receive the result from `-[TGMapView captureScreenshot:]`.

 @param mapView The map view instance.
 @param screenshot The image object representing the screenshot.
 */
- (void)mapView:(nonnull TGMapView *)mapView
didCaptureScreenshot:(nonnull UIImage *)screenshot;

/**
 Called after a scene has been loaded or updated.

 See:

 `-[TGMapView loadSceneAsyncFromURL:withUpdates:]`

 `-[TGMapView loadSceneFromYAML:relativeToURL:withUpdates:]`

 `-[TGMapView loadSceneAsyncFromYAML:relativeToURL:withUpdates:]`

 `-[TGMapView updateSceneAsync:]`

 @param mapView The map view instance.
 @param sceneID The ID of the scene that was loaded or updated.
 @param sceneError Any error encountered while loading or updating the scene.
 */
- (void)mapView:(nonnull TGMapView *)mapView didLoadScene:(int)sceneID withError:(nullable NSError *)sceneError;

/**
 Called after the view completes loading all content in the current view.

 @param mapView The map view instance.
 */
- (void)mapViewDidCompleteLoading:(nonnull TGMapView *)mapView;

@end // protocol TGMapViewDelegate

NS_ASSUME_NONNULL_BEGIN

/**
 The `TGGestureDelegate` protocol can be implemented to receive gesture events from the map view. The map view will
 first check whether a gestureDelegate is set, then check whether it responds to any `shouldRecognize*` method:

 - If the delegate responds to `shouldRecognize*`, the map view only performs its default handling of the gesture if
 `shouldRecognize*` returns `YES`.

 - If the delegate doesn't respond to `shouldRecognize*`, the map view performs its default handling of the gesture.

 Finally, if the delegate implements `didRecognize*` then the map view calls this method after the gesture is handled.

 @note These methods are all **optional**. All the screen positions in this interface are in _logical pixels_ or
 _drawing coordinate system_ (based on a `UIKit` coordinate system) which is independent of the phone pixel density.
 Refer to the
 <a href="https://developer.apple.com/library/content/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/GraphicsDrawingOverview/GraphicsDrawingOverview.html">
 Apple documentation</a> regarding _Coordinate Systems and Drawing in iOS_ for more informations.
 */
@protocol TGRecognizerDelegate <NSObject>
@optional

/**
 Whether the map view should handle a single tap gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param location The location of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizeSingleTapGesture:(CGPoint)location;

/**
 Whether the map view should handle a double tap gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param location The location of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizeDoubleTapGesture:(CGPoint)location;

/**
 Whether the map view should handle a long press gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param location The location of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizeLongPressGesture:(CGPoint)location;

/**
 Whether the map view should handle a pan gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param displacement The displacement of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizePanGesture:(CGPoint)displacement;

/**
 Whether the map view should handle a pinch gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param location The position of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizePinchGesture:(CGPoint)location;

/**
 Whether the map view should handle a rotation gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param location The position of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizeRotationGesture:(CGPoint)location;

/**
 Whether the map view should handle a shove gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @param displacement The displacement of the recognized gesture in the view.
 @return `YES` if the map view should handle this gesture, otherwise `NO`.
 */
- (BOOL)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
shouldRecognizeShoveGesture:(CGPoint)displacement;

/**
 If implemented, the returned value will be the focus for the rotation gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @return The screen position the rotation gesture should focus to.
 */
- (CGPoint)rotationFocus:(TGMapView *)view recognizer:(UIGestureRecognizer *)recognizer;

/**
 If implemented, the returned value will be the focus for the pinch gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` that recognized the gesture.
 @return The screen position the pinch gesture should focus to.
 */
- (CGPoint)pinchFocus:(TGMapView *)view recognizer:(UIGestureRecognizer *)recognizer;

/**
 Called when the map view just handled a single tap gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param location The position of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizeSingleTapGesture:(CGPoint)location;

/**
 Called when the map view just handled a single double tap gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param location The position of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
       recognizer:(UIGestureRecognizer *)recognizer
didRecognizeDoubleTapGesture:(CGPoint)location;

/**
 Called when the map view just handled a long press gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param location The position of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizeLongPressGesture:(CGPoint)location;

/**
 Called when the map view just handled a pan gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param displacement The displacement of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizePanGesture:(CGPoint)displacement;

/**
 Called when the map view just handled a pinch gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param location The position of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizePinchGesture:(CGPoint)location;

/**
 Called when the map view just handled a rotation gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param location The position of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizeRotationGesture:(CGPoint)location;

/**
 Called when the map view just handled a shove gesture.

 @param view The map view instance.
 @param recognizer The `UIGestureRecognizer` the recognized the gesture.
 @param displacement The displacement of the recognized gesture in the view.
 */
- (void)mapView:(TGMapView *)view
     recognizer:(UIGestureRecognizer *)recognizer
didRecognizeShoveGesture:(CGPoint)displacement;

@end // protocol TGGestureDelegate

NS_ASSUME_NONNULL_END
