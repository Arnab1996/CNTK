//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#pragma once

#include "CNTKLibrary.h"
#include "Learner.h"

namespace CNTK
{
    ///
    /// Base class for distributed learners.
    ///
    class DistributedLearnerBase : public DistributedLearner
    {
    public:
        const std::vector<Parameter>& Parameters() const override
        {
            return m_learner->Parameters();
        }

        void ResetSmoothedGradients() override
        {
            return m_learner->ResetSmoothedGradients();
        }

        Dictionary CreateCheckpoint() override;

        void RestoreFromCheckpoint(const Dictionary& checkpoint) override;

        void Shutdown() override {}

        DistributedCommunicatorPtr GetCommunicator() override
        {
            return m_communicator;
        }

        ///
        /// Sets a new learning rate overriding the schedule parameter used to construct this learner.
        ///
        void ResetLearningRate(const LearningRateSchedule& learningRateSchedule) override
        {
            m_learner->ResetLearningRate(learningRateSchedule);
        }

        ///
        /// Returns current learning rate.
        ///
        double LearningRate() const override
        {
            return m_learner->LearningRate();
        }

    protected:
        DistributedLearnerBase(DistributedCommunicatorPtr communicator, const std::vector<LearnerPtr>& learners, size_t distributeAfterSamples);

        static void PrepaireZeroGradients(std::vector<std::pair<Parameter, NDArrayViewPtr>>& gradientValues, MinibatchInfo& info);

        const DistributedCommunicatorPtr m_communicator;
        const CompositeLearnerPtr m_learner;
        const size_t m_distributeAfterSamples;

        size_t m_totalNumberOfSamplesSeen;

        DistributedLearnerBase(const DistributedLearnerBase&) = delete;
        DistributedLearnerBase& operator=(const DistributedLearnerBase&) = delete;
        DistributedLearnerBase& operator=(DistributedLearnerBase&&) = delete;
        DistributedLearnerBase(DistributedLearnerBase&& other) = delete;
    };
}