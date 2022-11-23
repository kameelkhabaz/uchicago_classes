'''
Linear regression

Kameel Khabaz

Main file for linear regression and model selection.
'''

import numpy as np
from sklearn.model_selection import train_test_split
import util


class DataSet(object):
    '''
    Class for representing a data set.
    '''

    def __init__(self, dir_path):
        '''
        Class for representing a dataset, performs train/test
        splitting.

        Inputs:
            dir_path: (string) path to the directory that contains the
              file
        '''

        parameters_dict = util.load_json_file(dir_path, "parameters.json")
        self.feature_idx = parameters_dict["feature_idx"]
        self.name = parameters_dict["name"]
        self.target_idx = parameters_dict["target_idx"]
        self.training_fraction = parameters_dict["training_fraction"]
        self.seed = parameters_dict["seed"]
        self.labels, data = util.load_numpy_array(dir_path, "data.csv")

        # do standardization before train_test_split
        if(parameters_dict["standardization"] == "yes"):
            data = self.standardize_features(data)

        self.training_data, self.testing_data = train_test_split(data,
            train_size=self.training_fraction, test_size=None,
            random_state=self.seed)

    # data standardization
    def standardize_features(self, data): 
        '''
        Standardize features to have mean 0.0 and standard deviation 1.0.
        Inputs:
          data (2D NumPy array of float/int): data to be standardized
        Returns (2D NumPy array of float/int): standardized data
        '''
        mu = data.mean(axis=0) 
        sigma = data.std(axis=0) 
        return (data - mu) / sigma

class Model(object):
    '''
    Class for representing a model.

    Attributes:
      target_idx: index of target variable
      feature_idx: list of integers containing indices of feature variables
      beta: NumPy array with model's best fit parameters (beta)
      R2: the R^2 value for the model
      training_data: data used to train the model
      labels: labels for dataset

    Methods:
      compute_R2(X, y): compute the R2 value for features X and target y using
        model's fit parameters beta
    '''

    def __init__(self, dataset, feature_idx):
        '''
        Construct a data structure to hold the model.
        Inputs:
            dataset: an dataset instance
            feature_idx: a list of the feature indices for the columns (of the
              original data array) used in the model.
        '''

        self.feature_idx = feature_idx
        self.target_idx = dataset.target_idx
        self.training_data = dataset.training_data
        self.labels = dataset.labels

        # Prepare data
        X = util.prepend_ones_column(self.training_data[:, feature_idx])
        y = self.training_data[:, self.target_idx]

        # Compute fit 
        self.beta = util.linear_regression(X, y)
        self.R2 = self.compute_R2(X, y)
        

    def compute_R2(self, X, y):
        '''
        Compute the coefficient of determination (R2 value) using this model's
        best fit parameters (beta) on given data
        
        Inputs:
          X (NumPy array): the matrix of observations of feature variable(s)
          y (NumPy array): array of observations of target variable

        Returns (float): the coefficient of determination (R2 value) 
        '''   

        predicted_y = util.apply_beta(self.beta, X)
        variance_residuals = np.sum((y - predicted_y) ** 2)
        variance_y = np.sum((y - y.mean()) ** 2)
        R2 = 1 - variance_residuals / variance_y

        return R2

    def __repr__(self):
        '''
        Format model as a string.
        '''
        s = "{} ~ {:.6f} ".format(self.labels[self.target_idx], self.beta[0])

        for i, feat in enumerate(self.feature_idx): 
            s += "+ {:6f} * {} ".format(self.beta[i + 1], self.labels[feat])
        
        return s

            


def compute_single_var_models(dataset):
    '''
    Computes all the single-variable models for a dataset

    Inputs:
        dataset: (DataSet object) a dataset

    Returns:
        List of Model objects, each representing a single-variable model
    '''

    return [Model(dataset, [idx]) for idx in dataset.feature_idx]


def compute_all_vars_model(dataset):
    '''
    Computes a model that uses all the feature variables in the dataset

    Inputs:
        dataset: (DataSet object) a dataset

    Returns:
        A Model object that uses all the feature variables
    '''

    return Model(dataset, dataset.feature_idx)


def compute_best_pair(dataset):
    '''
    Find the bivariate model with the best R2 value

    Inputs:
        dataset: (DataSet object) a dataset

    Returns:
        A Model object for the best bivariate model
    '''
    features = dataset.feature_idx
    best_model = Model(dataset,[]) # Create empty model

    for f1 in features:
        for f2 in features:
            if f1 >= f2:
                continue

            test_model = Model(dataset, [f1, f2])
            if test_model.R2 > best_model.R2:
                best_model = test_model
    
    return best_model


def forward_selection(dataset):
    '''
    Given a dataset with P feature variables, uses forward selection to
    select models for every value of K between 1 and P.

    Inputs:
        dataset: (DataSet object) a dataset

    Returns:
        A list (of length P) of Model objects. The first element is the
        model where K=1, the second element is the model where K=2, and so on.
    '''
    features = dataset.feature_idx
    best_models = []
    best_model = Model(dataset,[]) # Create empty model
    
    for _ in features:
        features_chosen = best_model.feature_idx

        # Find best new feature to add
        for f2 in features:
            if f2 in features_chosen:
                continue

            test_model = Model(dataset, features_chosen + [f2])

            if test_model.R2 > best_model.R2:
                best_model = test_model
                
        best_models.append(best_model)
    
    return best_models


def validate_model(dataset, model):
    '''
    Given a dataset and a model trained on the training data,
    compute the R2 of applying that model to the testing data.

    Inputs:
        dataset: (DataSet object) a dataset
        model: (Model object) A model that must have been trained
           on the dataset's training data.

    Returns:
        (float) An R2 value
    '''
    
    X = util.prepend_ones_column(dataset.testing_data[:, model.feature_idx])
    y = dataset.testing_data[:, model.target_idx]
    
    return model.compute_R2(X, y)