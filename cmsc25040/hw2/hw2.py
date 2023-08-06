import numpy as np
from hw1 import *
from visualize import * # for debugging
"""
   INTEREST POINT OPERATOR (12 Points Implementation + 3 Points Write-up)

   Implement an interest point operator of your choice.

   Your operator could be:

   (A) The Harris corner detector (Szeliski 7.1.1)

               OR

   (B) The Difference-of-Gaussians (DoG) operator defined in:
       Lowe, "Distinctive Image Features from Scale-Invariant Keypoints", 2004.
       https://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf

               OR

   (C) Any of the alternative interest point operators appearing in
       publications referenced in Szeliski or in lecture

              OR

   (D) A custom operator of your own design

   You implementation should return locations of the interest points in the
   form of (x,y) pixel coordinates, as well as a real-valued score for each
   interest point.  Greater scores indicate a stronger detector response.

   In addition, be sure to apply some form of spatial non-maximum suppression
   prior to returning interest points.

   Whichever of these options you choose, there is flexibility in the exact
   implementation, notably in regard to:

   (1) Scale

       At what scale (e.g. over what size of local patch) do you operate?

       You may optionally vary this according to an input scale argument.

       We will test your implementation at the default scale = 1.0, so you
       should make a reasonable choice for how to translate scale value 1.0
       into a size measured in pixels.

   (2) Nonmaximum suppression

       What strategy do you use for nonmaximum suppression?

       A simple (and sufficient) choice is to apply nonmaximum suppression
       over a local region.  In this case, over how large of a local region do
       you suppress?  How does that tie into the scale of your operator?

   For making these, and any other design choices, keep in mind a target of
   obtaining a few hundred interest points on the examples included with
   this assignment, with enough repeatability to have a large number of
   reliable matches between different views.

   If you detect more interest points than the requested maximum (given by
   the max_points argument), return only the max_points highest scoring ones.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

   Arguments:
      image       - a grayscale image in the form of a 2D numpy array
      max_points  - maximum number of interest points to return
      scale       - (optional, for your use only) scale factor at which to
                    detect interest points
      mask        - (optional, for your use only) foreground mask constraining
                    the regions to extract interest points
   Returns:
      xs          - numpy array of shape (N,) containing x-coordinates of the
                    N detected interest points (N <= max_points)
      ys          - numpy array of shape (N,) containing y-coordinates
      scores      - numpy array of shape (N,) containing a real-valued
                    measurement of the relative strength of each interest point
                    (e.g. corner detector criterion OR DoG operator magnitude)
"""
def find_interest_points(image, max_points = 200, scale = 1.0, mask = None):
   # check that image is grayscale
   assert image.ndim == 2, 'image should be grayscale'
   ##########################################################################
   # Implement Harris corner detector
   [img_r, img_c] = image.shape

   dx, dy = sobel_gradients(image) # per-pixel gradients

   # Calculate second derivatives 
   # Convolve by applying a Gaussian over scale pixels 
   sigma = scale
   dxdx = denoise_gaussian(dx * dx, sigma = sigma)
   dxdy = denoise_gaussian(dx * dy, sigma = sigma)
   dydy = denoise_gaussian(dy * dy, sigma = sigma)

   # Get candidate corners
   alpha = 0.05 # constant 
   cand_xs = np.array([])
   cand_ys = np.array([])
   Rs = np.zeros((img_r, img_c))
   for y in range(img_r):
      for x in range(img_c):
         if mask is not None and not mask[y,x]:
            continue # don't extract if not in mask 
         Tdxdx = dxdx[y, x]
         Tdxdy = dxdy[y, x]
         Tdydy = dydy[y, x]
         R = Tdxdx * Tdydy - (Tdxdy * Tdxdy) - \
             alpha * (Tdxdx + Tdydy) * (Tdxdx + Tdydy)
         if R > 0: # corner or flat
            Rs[y, x] = R
            cand_xs = np.append(cand_xs, x)
            cand_ys = np.append(cand_ys, y)

   # Nonmaximum suppression
   corners = np.zeros((img_r,img_c))
   scale = int(np.ceil(scale))
   for y, x in zip(cand_ys, cand_xs):
      y = int(y)
      x = int(x)
      miny = max(y - scale, 0)
      minx = max(x - scale, 0)
      maxy = min(y + scale + 1, img_r)
      maxx = min(x + scale + 1, img_c)

      if np.all(np.greater_equal(Rs[y, x], Rs[miny:maxy, minx: maxx])):
         corners[y, x] = Rs[y, x]
   
   # Threshold 
   nz = np.count_nonzero(corners)
   if nz > max_points:
      t = np.percentile(corners[corners != 0], 100 - 100*max_points / nz)
   else: 
      t = 0
   
   (ys, xs) = np.where(corners > t)
   scores = corners[ys, xs]
   ##########################################################################
   return xs, ys, scores

"""
   FEATURE DESCRIPTOR (12 Points Implementation + 3 Points Write-up)

   Implement a SIFT-like feature descriptor by binning orientation energy
   in spatial cells surrounding an interest point.

   Unlike SIFT, you do not need to build-in rotation or scale invariance.

   A reasonable default design is to consider a 3 x 3 spatial grid consisting
   of cell of a set width (see below) surrounding an interest point, marked
   by () in the diagram below.  Using 8 orientation bins, spaced evenly in
   [-pi,pi), yields a feature vector with 3 * 3 * 8 = 72 dimensions.

             ____ ____ ____
            |    |    |    |
            |    |    |    |
            |____|____|____|
            |    |    |    |
            |    | () |    |
            |____|____|____|
            |    |    |    |
            |    |    |    |
            |____|____|____|

                 |----|
                  width

   You will need to decide on a default spatial width.  Optionally, this can
   be a multiple of a scale factor, passed as an argument.  We will only test
   your code by calling it with scale = 1.0.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

  Arguments:
      image    - a grayscale image in the form of a 2D numpy
      xs       - numpy array of shape (N,) containing x-coordinates
      ys       - numpy array of shape (N,) containing y-coordinates
      scale    - scale factor

   Returns:
      feats    - a numpy array of shape (N,K), containing K-dimensional
                 feature descriptors at each of the N input locations
                 (using the default scheme suggested above, K = 72)
"""
def extract_features(image, xs, ys, scale = 1.0):
   # check that image is grayscale
   assert image.ndim == 2, 'image should be grayscale'
   ##########################################################################
   nbins = 8
   grid_len = 3
   gradient_threshold = 0.2 # fraction of the sum of the gradients

   N = len(xs)
   K = grid_len * grid_len * nbins
   hist_bin_edges = np.linspace(-np.pi, np.pi, nbins + 1)

   # Need odd scale 
   if scale % 2 == 0:
      scale += 1

   cell_width = int(scale) * 3
   padd_amt = cell_width + (cell_width // 2)
   padded_image = mirror_border(image, wx = padd_amt, wy = padd_amt)

   # Apply a small blur to smooth out noise 
   simage = denoise_gaussian(padded_image, sigma = scale)
   dx, dy = sobel_gradients(simage) # per-pixel gradients
   grad_mag = linalg.norm(np.dstack((dx, dy)), axis = 2)
   theta = np.arctan2(dy, dx) # output in range [-pi, pi]


   feats = np.zeros((N, K))
   for pt_num, (y, x) in enumerate(zip(ys, xs)):
         # change to padded image coordinates
         py = int(y) + padd_amt
         px = int(x) + padd_amt
         # calculate window bounds
         miny = py - padd_amt
         minx = px - padd_amt
         # max is non-inclusive 
         maxy = py + padd_amt + 1
         maxx = px + padd_amt + 1
         # size is (cell_width * 3, cell_width * 3) 
         window_thetas = theta[miny: maxy, minx: maxx]
         window_grad_mag = grad_mag[miny: maxy, minx: maxx]

         # split window thetas/gradient magnitudes into 3 rows
         window_theta_rows = np.vsplit(window_thetas,3)
         window_grad_mag_rows = np.vsplit(window_grad_mag, 3)

         # get interest point histograms at each grid position
         interest_pt_hists = np.array([])
         for i in range(grid_len):
            col_thetas = np.hsplit(window_theta_rows[i], 3)
            col_grad_mag = np.hsplit(window_grad_mag_rows[i],3)

            for j in range(grid_len):
               cell_thetas = col_thetas[j]
               cell_grad_mag = col_grad_mag[j]

               # clamp excessively high gradients
               grad_sum = np.sum(cell_grad_mag)
               thresh = gradient_threshold * grad_sum
               cell_grad_mag[cell_grad_mag > thresh] = thresh

               # get histogram (normalizes integral to 1)
               h, _ = np.histogram(cell_thetas, bins = hist_bin_edges, weights = cell_grad_mag, density = True)
               interest_pt_hists = np.append(interest_pt_hists, h)

         # Assign feature descriptors for interest point
         feats[pt_num,:] = interest_pt_hists

   ##########################################################################
   return feats


"""
   FEATURE MATCHING (7 Points Implementation + 3 Points Write-up)

   Given two sets of feature descriptors, extracted from two different images,
   compute the best matching feature in the second set for each feature in the
   first set.

   Matching need not be (and generally will not be) one-to-one or symmetric.
   Calling this function with the order of the feature sets swapped may
   result in different returned correspondences.

   For each match, also return a real-valued score indicating the quality of
   the match.  This score could be based on a distance ratio test, in order
   to quantify distinctiveness of the closest match in relation to the second
   closest match.  It could optionally also incorporate scores of the interest
   points at which the matched features were extracted.  You are free to
   design your own criterion.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

   Arguments:
      feats0   - a numpy array of shape (N0, K), containing N0 K-dimensional
                 feature descriptors (generated via extract_features())
      feats1   - a numpy array of shape (N1, K), containing N1 K-dimensional
                 feature descriptors (generated via extract_features())
      scores0  - a numpy array of shape (N0,) containing the scores for the
                 interest point locations at which feats0 was extracted
                 (generated via find_interest_point())
      scores1  - a numpy array of shape (N1,) containing the scores for the
                 interest point locations at which feats1 was extracted
                 (generated via find_interest_point())

   Returns:
      matches  - a numpy array of shape (N0,) containing, for each feature
                 in feats0, the index of the best matching feature in feats1
      scores   - a numpy array of shape (N0,) containing a real-valued score
                 for each match
"""
def match_features(feats0, feats1, scores0, scores1):
   ##########################################################################
   N0 = len(feats0)
   N1 = len(feats1)
   nbins = len(feats0[0,:])
   matches = np.zeros((N0,), dtype = int)
   scores = np.zeros((N0,))


   for i in range(N0):
      feat = feats0[i,:]
      # Use a Chi-Square Two Sample test to compute feature distances
      # see https://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/chi2samp.htm
      rep_feat = np.repeat(feat.reshape(1,-1), N1, axis = 0)
      sums_feat = np.sum(rep_feat, axis = 1)
      sums_feats1 = np.sum(feats1, axis = 1)
      # scaling constants
      K1 = np.sqrt(sums_feats1 / sums_feat)
      K1_m = np.repeat(K1.reshape(-1,1), nbins, axis = 1)
      K2 = np.sqrt(sums_feat / sums_feats1)
      K2_m = np.repeat(K2.reshape(-1,1), nbins, axis = 1)
      # terms 
      feat_sum = rep_feat + feats1
      feat_sqr_diff = np.square(K1_m * rep_feat - K2_m * feats1)

      # Calculate quotient 
      quot = np.divide(feat_sqr_diff, feat_sum, \
                       out = np.zeros_like(feat_sum), where = feat_sum != 0)
      distances = np.sum(quot, axis = 1)

      idcs = np.argsort(distances)

      matches[i] = idcs[0]
      # compute nndr between closest and second closest point 
      nndr = distances[idcs[0]] / distances[idcs[1]]
      scores[i] = 1 / nndr
   ##########################################################################
   return matches, scores

"""
   HOUGH TRANSFORM (7 Points Implementation + 3 Points Write-up)

   Assuming two images of the same scene are related primarily by
   translational motion, use a predicted feature correspondence to
   estimate the overall translation vector t = [tx ty].

   Your implementation should use a Hough transform that tallies votes for
   translation parameters.  Each pair of matched features votes with some
   weight dependant on the confidence of the match; you may want to use your
   estimated scores to determine the weight.

   In order to accumulate votes, you will need to decide how to discretize the
   translation parameter space into bins.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

   Arguments:
      xs0     - numpy array of shape (N0,) containing x-coordinates of the
                interest points for features in the first image
      ys0     - numpy array of shape (N0,) containing y-coordinates of the
                interest points for features in the first image
      xs1     - numpy array of shape (N1,) containing x-coordinates of the
                interest points for features in the second image
      ys1     - numpy array of shape (N1,) containing y-coordinates of the
                interest points for features in the second image
      matches - a numpy array of shape (N0,) containing, for each feature in
                the first image, the index of the best match in the second
      scores  - a numpy array of shape (N0,) containing a real-valued score
                for each pair of matched features

   Returns:
      tx      - predicted translation in x-direction between images
      ty      - predicted translation in y-direction between images
      max_votes - the max value in the matrix storing vote tallies;
                  this output is provided for your own convenience and 
                  you are free to design its format
"""
def hough_votes(xs0, ys0, xs1, ys1, matches, scores):
   ##########################################################################
   # find translation from image 0 to image 1 
   N0 = len(xs0)

   tx_bins = 20
   ty_bins = 20
   # Store translation vectors between interest points 
   txs = np.zeros(N0)
   tys = np.zeros(N0)

   for i in range(N0):
      j = matches[i]
      txs[i] = xs1[j] - xs0[i]
      tys[i] = ys1[j] - ys0[i]

   min_tx = txs.min()
   max_tx = txs.max()

   min_ty = tys.min()
   max_ty = tys.max()

   tx_edges, _ = np.linspace(min_tx, max_tx, tx_bins, endpoint = False, retstep = True) 
   ty_edges, _ = np.linspace(min_ty, max_ty, ty_bins, endpoint = False, retstep = True)
   
   # tally votes
   votes = np.zeros((ty_bins, tx_bins))
   for i in range(N0):
      j = matches[i]
      # take min with bins - 1 in case the value equals the maximum
      tx_bin = max(np.where(txs[i] >= tx_edges)[0])
      ty_bin = max(np.where(tys[i] >= ty_edges)[0])
      votes[ty_bin, tx_bin] += scores[i]

   # find best tx and ty
   tyi, txi = np.unravel_index(np.argmax(votes), votes.shape)
   ty = ty_edges[tyi]
   tx = tx_edges[txi]
   max_votes = np.max(votes)
   ##########################################################################
   return tx, ty, max_votes

"""
    OBJECT DETECTION (10 Points Implementation + 5 Points Write-up)

    Implement an object detection system which, given multiple object
    templates, localizes the object in the input (test) image by feature
    matching and hough voting.

    The first step is to match features between template images and test image.
    To prevent noisy matching from background, the template features should
    only be extracted from foreground regions.  The dense point-wise matching
    is then used to compute a bounding box by hough voting, where box center is
    derived from voting output and the box shape is simply the size of the
    template image.

    To detect potential objects with diversified shapes and scales, we provide
    multiple templates as input.  To further improve the performance and
    robustness, you are also REQUIRED to implement a multi-scale strategy
    either:
       (a) Implement multi-scale interest points and feature descriptors OR
       (b) Repeat a single-scale detection procedure over multiple image scales
           by resizing images.

    In addition to your implementation, include a brief write-up (in hw2.pdf)
    of your design choices on multi-scale implementaion and samples of
    detection results (please refer to display_bbox() function in visualize.py).

    Arguments:
        template_images - a list of gray scale images.  Each image is in the
                          form of a 2d numpy array which is cropped to tightly
                          cover the object.

        template_masks  - a list of binary masks having the same shape as the
                          template_image.  Each mask is in the form of 2d numpy
                          array specyfing the foreground mask of object in the
                          corresponding template image.

        test_img        - a gray scale test image in the form of 2d numpy array
                          containing the object category of interest.

    Returns:
         bbox           - a numpy array of shape (4,) specifying the detected
                          bounding box in the format of
                             (x_min, y_min, x_max, y_max)

"""
def object_detection(template_images, template_masks, test_img):
   ##########################################################################
   # hyperparameters for finding interest points
   max_points = 100
   N_temp = len(template_images)
   # allow for more interest points from main image since object isn't cropped 
   test_xs, test_ys, test_scores = find_interest_points(test_img, max_points= 2 * max_points, \
                                                        scale= 1)
   test_feats = extract_features(test_img, test_xs, test_ys, scale=1)

   # hyperparameter for multi-scale strategy
   scale_factors = (0.5, 1, 2)
   N_scale = len(scale_factors)

   # each row is for a particular template-target match
   # store tx, ty, and hough score
   ts = np.zeros((N_temp * N_scale, 3))
   temp_shapes = np.zeros((N_temp * N_scale, 2))
   for i, (orig_temp_img, orig_temp_mask) in enumerate(zip(template_images, template_masks)):

      for j, scale_factor in enumerate(scale_factors):
         idx = i * N_scale + j
         # scale image depending on scale factor
         if scale_factor < 1:
            downsample_factor = int(1 / scale_factor)
            temp_img = smooth_and_downsample(orig_temp_img, downsample_factor)
            temp_mask = orig_temp_mask[::downsample_factor, ::downsample_factor]
         elif scale_factor == 1:
            temp_img = orig_temp_img
            temp_mask = orig_temp_mask
         else: # upsample
            if np.any(np.multiply(scale_factor, orig_temp_img.shape) > test_img.shape):
               # template image cant be larger than test image
               continue

            temp_img = bilinear_upsampling(orig_temp_img, scale_factor)
            temp_mask = bilinear_upsampling(orig_temp_mask, scale_factor)
            temp_mask = np.where(temp_mask > 0.5, 1, 0)

         temp_shapes[idx,:] = temp_img.shape 

         temp_xs, temp_ys, temp_scores = find_interest_points(temp_img, max_points=max_points, \
                                                              scale = 1, mask=temp_mask)

         temp_feats = extract_features(temp_img, temp_xs, temp_ys, scale = 1)

         # match template to test
         matches, match_scores = match_features(temp_feats, test_feats, \
                                             temp_scores, test_scores)
      
         ts[idx] = hough_votes(temp_xs, temp_ys, test_xs, test_ys, \
                                       matches, match_scores)

         # Test code for debugging
         # plot_interest_points(temp_img, temp_xs, temp_ys, temp_scores)
         # plot_matches(temp_img, test_img, temp_xs, temp_ys, test_xs, test_ys, matches, match_scores, 0)
   
   best_t_idx = np.argmax(ts[:,2])
   x_min = ts[best_t_idx, 0]
   y_min = ts[best_t_idx, 1]
   y_size, x_size = temp_shapes[best_t_idx, :]

   x_max = x_min + x_size
   y_max = y_min + y_size

   test_y_size, test_x_size = test_img.shape
   if x_min < 0:
      x_max -= x_min
      x_min = 0
   elif x_max > test_x_size - 1:
      x_min -= (x_max - (test_x_size - 1))
      x_max = test_x_size - 1
      
   if y_min < 0:
      y_max -= y_min
      y_min = 0
   elif y_max > test_y_size - 1:
      y_min -= (y_max - (test_y_size - 1))
      y_max = test_y_size - 1



   bbox = [x_min, y_min, x_max, y_max]

   ##########################################################################
   return bbox
