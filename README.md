# ofxSelfOrganizingMap 

ofxSelfOrganizingMap an addon for [openFrameworks](http://openframeworks.cc/) which implements a self-organizing map (SOM) or Kohonen map, an [unsupervised machine learning](http://en.wikipedia.org/wiki/Unsupervised_learning) algorithm for mapping high-dimensional data onto a low-dimensional feature space. SOMs have many applications in data clustering, compression, and pattern recognition. 

For more information about SOMs, see:
* [http://en.wikipedia.org/wiki/Self-organizing_map](http://en.wikipedia.org/wiki/Self-organizing_map)
* [http://www.ai-junkie.com/ann/som/som1.html](http://www.ai-junkie.com/ann/som/som1.html)

An example application: Mining different words/phrases with Google Images and visualizing the color distributions via SOM.
* [http://genekogan.com/works/color-of-words.html](http://genekogan.com/works/color-of-words.html)


## Usage

To instantiate a self-organizing map (SOM), construct an ofxSelfOrganizingMap object. The SOM is initialized with its default properties: the size of the map (output nodes) is set to 256 x 256 x 1. The SOM takes two parameters, the initial learning rate, and the number of iterations, which are defaulted to 0.1 and 3000, respectively. They may be changed with the following methods.

	// instantiate SOM 
	ofxSelfOrganizingMap som;
	som.setMapSize(300,300);		// map is 2D
	som.setMapSize(300,300,300);	// this makes the map 3D
	som.setInitialLearningRate(0.2);
	som.setNumIterations(5000);
	
Before using the SOM, you **must** tell it the size of the input feature vectors and their min and max values, so it can normalize to avoid some features dominating the computation.

	// tell the SOM to expect 5-dimensional feature vectors bounded by minInstance and maxInstance
	double minInstance[5] = { 0, 0, 0, 0, 0 };
	double maxInstance[5] = { 255, 255, 255, 1, 1 };
	som.setFeaturesRange(5, minInstance, maxInstance);
	
Finally, call `setup()` to allocate space for the SOM. Don't forget to do this! Calling it again returns the SOM to the same initial state and can be called as many times as you want.

	som.setup();
	
To train the SOM, you must give it as many sample feature vectors as specified by the `numInstances` parameter. Generally, you should train the SOM by randomly sampling from the feature space. The SOM will ignore instances sent to it past the first `numIterations`.

	// do this som.numIterations times!
	double instance[5] = { 127, 50, 200, 0.3, 0.9 };	// random sample from your training set
	som.updateMap(instance);

Ideally, a well-trained map has the useful property that different regions of it contain very similar feature vectors in roughly the same proportions as found in the initial training set. This makes it useful for clustering data points, and visualizing feature distributions.

Finally, to use the now-trained SOM, you can get the weight vector at any point in the map, or find the point on the map to which a given instance is most similar.
	
	// w is an array holding the value of the map at the point queried
	double *w = som.getMapAt(150, 120, 30);	
	
	// to get nearest point for some instance
	double instance[5] = { 127, 50, 200, 0.3, 0.9 };
	ofPoint nearestPt = findBestMatchingCell(instance);


## Tips

Usually, the default parameters for the SOM are fine for most use cases.  If you find that the resulting map is not smooth in some regions, or too noisy/grainy, then you can try training it more aggressively by either increasing the the initial learning rate or the number of iterations.  

Using a high learning rate may cause the map to converge on less optimal results, but can be trained in fewer iterations, which can be useful if you need it to run faster.


## Examples

### The colors of the seasons, visualized using an SOM over google image search colors

![http://www.genekogan.com/experiments/color-of-words.html](http://www.genekogan.com/images/colors-of-words/seasons.jpg)

### Countries clustered via UN poverty statistics (via [AI-junkie](http://ai-junkie.com/))
w
