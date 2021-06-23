# Base utils
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Feature selection
from sklearn.feature_selection import SelectKBest, f_classif
from sklearn.feature_selection import RFE

# Imputation
from sklearn.impute import SimpleImputer

# PCA
from sklearn.decomposition import PCA

# Models
from sklearn.neural_network import MLPClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn import tree

# Cross validation
from sklearn.model_selection import cross_validate
from sklearn.model_selection import StratifiedKFold
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import StratifiedKFold
from sklearn.model_selection import train_test_split

# Pipeline
from sklearn.pipeline import make_pipeline
from sklearn import preprocessing
from sklearn.pipeline import Pipeline

# Base classes for custom estimators
from sklearn.base import BaseEstimator
from sklearn.base import TransformerMixin

from sklearn.metrics import f1_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import confusion_matrix
from sklearn.metrics import plot_confusion_matrix


class CorrelationMatrixTransformer(BaseEstimator, TransformerMixin):
    """
    Class representing correlation matrix transformer used for feature selection.
    Overrides BaseEstimator methods to drop features with higher correlation than
    parameter "corr".
    """

    def __init__(self, corr=0.9):
        self.to_drop = []
        self.corr = corr
        pass

    def fit(self, X, y=None):
        correlated_features = set()
        correlation_matrix = X.corr()
        for i in range(len(correlation_matrix.columns)):
            for j in range(i):
                if abs(correlation_matrix.iloc[i, j]) >= self.corr:
                    colname = correlation_matrix.columns[i]
                    correlated_features.add(colname)
        self.to_drop = list(correlated_features)
        return self

    def transform(self, X, y=None):
        X_ = X.copy()
        return X_.drop(self.to_drop, axis=1)


def get_report(dataset):
    """
    Function to get report from cross validation tests.
    It shows:
        Mean values for each [preprocessor and classifier] combination
        Max values for each [preprocessor and classifier] combination

        Mean values for each preprocessor
        Mean values for each classifier

        Top 10 rounds for each metric

        Graphs displaying correlation between metrics and prior fraud probabilities
    """

    dataset = dataset.round(2)
    print('Overall results (mean): ')
    display(dataset[['classifier', 'preprocessor', 'f1', 'precision', 'recall']].groupby(['preprocessor', 'classifier'])
            .mean().round(2))
    print('Overall results (max): ')
    display(dataset[['classifier', 'preprocessor', 'f1', 'precision', 'recall']].groupby(['preprocessor', 'classifier'])
            .max().round(2))
    print('Grouped by Preprocessor (mean):')
    display(dataset[['preprocessor', 'f1', 'precision', 'recall']].groupby('preprocessor').mean().round(2))
    print('Grouped by Classifier (mean):')
    display(dataset[['classifier', 'f1', 'precision', 'recall']].groupby('classifier').mean().round(2))

    preprocessors = dataset['preprocessor'].unique()
    metrics = ['f1', 'precision', 'recall']

    # For each metric, display top 10 rounds.
    for m in metrics:
        print(f'Top 10 by {m}:')
        display(dataset.sort_values(m, ascending=False).head(10).round(2))

    for p in preprocessors:
        for m in metrics:
            d = dataset[dataset['preprocessor'] == p]
            for c in dataset['classifier'].unique():
                plt.plot(d[d['classifier'] == c]['prior'].unique(), d[d['classifier'] == c].groupby('prior').mean()[m],
                         label=str(c))
            plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
            plt.title(m + ' - ' + str(p))
            plt.show()


def _make_pipeline(preprocessors, classifier):
    """
    Custom pipeline maker to allow chaining more than 1 preprocessor (max. 2)
    """
    if isinstance(preprocessors, list):
        # support only preprocessing of lenght 2
        return make_pipeline(preprocessors[0], preprocessors[1], classifier)
    if preprocessors is None:
        return make_pipeline(classifier)

    return make_pipeline(preprocessors, classifier)


def stratified_k_fold(data, aditional_preprocessors, dataset_description, verbose=False, f_type='charged_off',
                      n_rounds=1):
    """
    Stratified k-fold with undersampling. Please see section 3.1 in the thesis for further information
    :param aditional_preprocessors: Preprocessors to fit on training folds.
    :param dataset_description: Last column of returned result
    :param verbose: Controls verbsity of this algorithm.
    :param f_type: Charged off clients or marked clients
    :param n_rounds: Number of rounds to repeat the whole cross validation.
    :returns DataFrame with results for each prior, preprocessor, classifier combination.
    For each combination, It takes mean of scores computed by the cross validation.

    Please note that this function is extremely time consuming as it runs 10 fold cross validation.
    Computation time for 5 priors, 5 classifiers and 16,000 rows and 160 features was around 5 minutes
    even though it used all available PCU cores (n_jobs = -1). With some preprocessing methods, the run
    took about 12 hours as for example RFE is extremely CPU demanding. Use this function with caution.
    """
    # Always compare with no preprocessor
    aditional_preprocessors.append(None)
    # List of used classifiers
    clf_mlp = MLPClassifier(random_state=12)
    clf_tree = tree.DecisionTreeClassifier(random_state=12)
    clf_svc = SVC(random_state=12)
    clf_r_forrest = RandomForestClassifier(random_state=12)
    clf_log = LogisticRegression(random_state=12)
    clfs = [clf_mlp, clf_tree, clf_svc, clf_r_forrest, clf_log]

    scoring = {'f1': 'f1', 'precision': 'precision', 'rec': 'recall'}
    # Defined priors
    priors = [0.05, 0.1, 0.25, 0.5]
    if f_type != 'charged_off':
        priors = [0.01, 0.05, 0.1, 0.25, 0.5]

    ret = pd.DataFrame(columns=['classifier', 'preprocessor', 'prior', 'f1', 'precision', 'recall'])
    for i in range(n_rounds):
        for p in priors:
            if verbose:
                print(f'Prior: {p}')
            for clf in clfs:
                for ap in aditional_preprocessors:
                    pipeline = _make_pipeline(ap, clf)
                    scores = cross_validate(pipeline, data.drop('fraud', axis=1), data['fraud'],
                                            cv=UndersampleStratifiedKFold(n_splits=10, prior=p), scoring=scoring,
                                            n_jobs=-1)
                    f1 = scores['test_f1'].mean()
                    prec = scores['test_precision'].mean()
                    rec = scores['test_rec'].mean()
                    if verbose:
                        print(pipeline)
                        print(f1)
                        print(prec)
                        print(rec)
                    ret = pd.concat([ret, pd.DataFrame(
                        columns=['classifier', 'preprocessor', 'prior', 'f1', 'precision', 'recall'],
                        data=[[clf, ap, p, f1, prec, rec]])], ignore_index=True)
    ret['dataset_description'] = dataset_description
    return ret


def distribution(vals):
    """
    Returns sorted digits distribution based on their frequency.
    """
    tot = 0
    rv = []
    for v in vals:
        tot += v
    for v in vals:
        rv.append(v * 100 // tot)
    # rv.extend(sorted(rv))
    return tuple(rv)


def digit_dist(row):
    """
    Parses the digits and returns their distribution.
    """
    rv = {}
    for i in range(10):
        rv[str(i)] = 0
    for i in row:
        i = str(i)
        for c in i:
            if c.isdigit():
                rv[c] = rv.get(c, 0) + 1
    return distribution(rv.values())


def plot_corr_matrix(df):
    """
    Plots correlation matrix of given DataFrame.
    """
    f = plt.figure(figsize=(19, 15))
    plt.matshow(df.corr().abs(), fignum=f.number)
    cb = plt.colorbar()
    cb.ax.tick_params(labelsize=14)
    plt.title('Correlation Matrix', fontsize=16)


class UndersampleStratifiedKFold:
    """
    Class for generating undersampled train folds for cross validation based on given prior.
    Modified implementation from this thread (author used oversampling instead)
    https://stackoverflow.com/questions/30040597/how-to-generate-a-custom-cross-validation-generator-in-scikit-learn
    """

    def __init__(self, n_splits=10, prior=0.05):
        self.n_splits = n_splits
        self.prior = prior

    def split(self, X, y, groups=None):
        """
        Generate stratified folds.
        """
        for rx, tx in StratifiedKFold(n_splits=self.n_splits).split(X, y):
            nix = np.where(y[rx] == 0)[0]
            pix = np.where(y[rx] == 1)[0]
            p_len = len(pix)
            # Compute size of majority class to match the prior
            maj_size = round(p_len * (1 - self.prior) / self.prior)
            # Perform random undersampling
            nixu = np.random.choice(nix, size=maj_size, replace=False)
            ix = np.append(nixu, pix)
            rxm = rx[ix]
            yield rxm, tx

    def get_n_splits(self, X, y, groups=None):
        return self.n_splits


def tune_params(classifier, grid, prior, X_train, y_train, save_folder, n_iter=100):
    """
    Parameter tuning by stratified k fold.
    """
    scoring = ['f1', 'precision', 'recall']
    # Stratified 5 fold cross validation
    res = RandomizedSearchCV(classifier, grid, refit=False, n_iter=n_iter,
                             cv=UndersampleStratifiedKFold(n_splits=5, prior=prior),
                             scoring=scoring, verbose=3, n_jobs=-1)
    res.fit(X_train, y_train)

    df = pd.concat([pd.DataFrame(res.cv_results_["params"]),
                    pd.DataFrame(res.cv_results_["mean_test_precision"], columns=["prec"]),
                    pd.DataFrame(res.cv_results_["mean_test_f1"], columns=["f1"]),
                    pd.DataFrame(res.cv_results_["mean_test_recall"], columns=["recall"])], axis=1)

    df.to_csv(save_folder)
    display(df)
    return df


def split(data):
    # Custom split for input and target for our scenario.
    return data.drop('fraud', axis=1), data['fraud']


def test_model(X_train, X_test, y_train, y_test, classifier, prior):
    # Basic metrics for classifier.
    nix = np.where(y_train[y_train.index] == 0)[0]
    pix = np.where(y_train[y_train.index] == 1)[0]
    p_len = len(pix)
    maj_size = round(p_len * (1 - prior) / prior)
    nixu = np.random.choice(nix, size=maj_size, replace=False)
    ix = np.append(nixu, pix)
    X_train = X_train.iloc[ix]
    y_train = y_train.iloc[ix]
    classifier.fit(X_train, y_train)
    y_pred = classifier.predict(X_test)
    print(f'F1 score: {round(f1_score(y_test, y_pred), 2)}')
    print(f'Precision score: {round(precision_score(y_test, y_pred), 2)}')
    print(f'Recall score: {round(recall_score(y_test, y_pred), 2)}')
    plot_confusion_matrix(classifier, X_test, y_test, colorbar=False)


"""
Parameters grid definition for classifiers.
"""
ann_space = {
    'hidden_layer_sizes': [(50,50,50), (50,100,50), (100,), (20, 40, 10)],
    'activation': ['tanh', 'relu', 'logistic'],
    'solver': ['sgd', 'adam', 'lbfgs'],
    'alpha': [0.0001, 0.05, 0.001, 0.01],
    'learning_rate': ['constant','adaptive'],
    'max_iter': [200, 400]
}

svc_space = {
    'C': [0.1, 0.5, 1, 2, 4],
    'kernel': ['rbf', 'poly', 'sigmoid', 'linear'],
    'degree':[2, 3, 5],
    'gamma':['scale', 'auto'],
    'class_weight':['balanced', None],
    'decision_function_shape':{'ovo', 'ovr'}
}

forest_grid = {'n_estimators': [10, 50, 100, 200],
               'max_features': ['auto', 'sqrt'],
               'max_depth': [10, 50, None],
               'min_samples_split': [2, 10],
               'min_samples_leaf': [1, 2, 4],
               'bootstrap': [True, False],
               'criterion': ['gini', 'entropy'],
               'class_weight': ['balanced', 'balanced_subsample', None]
               }

log_space = {
    'penalty': ['l1', 'l2', 'elasticnet', 'none'],
    'C': [0.1, 0.5, 1, 2, 4],
    'fit_intercept': [True, False],
    'class_weight': ['balanced', None],
    'solver': ['newton-cg', 'lbfgs', 'liblinear', 'sag', 'saga'],
    'mutli_class': ['auto', 'ovr', 'multinomial']

}

