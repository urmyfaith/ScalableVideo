#include "learninginterface.h"

LearningInterface::LearningInterface() :
    m_trainer(svm_c_trainer<kernel_type>()),
    m_is_trained(false)
{
}

//Add a training sample
void LearningInterface::addTrainingSample(FeatureSet &features, double label)
{
    sample_type samp;
//    samp(0) = (double)features.bandwidth();
//    samp(1) = (double)features.contentType();
    for(int i = 0; i < FEATURE_SET_NUM_FEATURES; i++)
        samp(i) = features.getFeatureByIndex((FeatureSetIndexType) i);

    m_training_set.push_back(samp);
    if(label == 1.0)
        m_labels.push_back(+1);
    else
        m_labels.push_back(-1);
}

void LearningInterface::trainCurrent()
{
    //Normalize the training set.
    m_normalizer.train(m_training_set);
    for (unsigned long i = 0; i < m_training_set.size(); ++i)
        m_training_set[i] = m_normalizer(m_training_set[i]);

    //Set C, epsilon and cache size
    m_trainer.set_c(0.5);
    m_trainer.set_epsilon(0.0001);
    //For now just leave defaults

    //Determine a good value for gamma (TODO)
    m_trainer.set_kernel(kernel_type(0.1));
    m_decision_function.normalizer = m_normalizer;

    //Train
    m_decision_function.function = m_trainer.train(m_training_set, m_labels);

    m_is_trained = true;

}

//Make a prediction using the given feature set
double LearningInterface::predict(FeatureSet &features)
{
    //Get the sample
    sample_type sample;
//    sample(0) = (double)features.bandwidth();
//    sample(1) = (double)features.contentType();
    for(int i = 0; i < FEATURE_SET_NUM_FEATURES; i++)
        sample(i) = features.getFeatureByIndex((FeatureSetIndexType) i);

    return m_decision_function(sample);

}
