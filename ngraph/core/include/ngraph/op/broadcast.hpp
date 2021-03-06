//*****************************************************************************
// Copyright 2017-2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#pragma once

#include "ngraph/axis_set.hpp"
#include "ngraph/op/op.hpp"
#include "ngraph/op/util/attr_types.hpp"
#include "ngraph/op/util/broadcast_base.hpp"

namespace ngraph
{
    namespace op
    {
        namespace v3
        {
            /// \brief Operation which "adds" axes to an input tensor, replicating elements from the
            ///        input as needed along the new axes.
            class NGRAPH_API Broadcast : public util::BroadcastBase
            {
            public:
                static constexpr NodeTypeInfo type_info{"Broadcast", 3};
                const NodeTypeInfo& get_type_info() const override { return type_info; }
                /// \brief Constructs a broadcast operation.
                Broadcast() = default;
                /// \brief Constructs a broadcast operation.
                ///
                /// \param arg            The input tensor to be broadcast.
                /// \param target_shape   The shape of the output tensor.
                /// \param axes_mapping   The axis positions (0-based) in the result that correspond
                ///                       to input axes. 'Arg' tensor is broadcast along the
                ///                       remaining axes.
                ///                       E.g., Input Shape - [3, 4], Target Shape - [3, 5, 4, 4]
                ///                       axes_mapping - [0, 2] => Broadcast along axes 1 and 3.
                ///                       axes_mapping - [0, 3] => Broadcast along axes 1 and 2.
                /// \param broadcast_spec Broadcast specification to use for determining broadcast
                ///                       axes. 'axes_mapping' should not be provided if mode other
                ///                       than explicit (none) is used.
                Broadcast(const Output<Node>& arg,
                          const Output<Node>& target_shape,
                          const Output<Node>& axes_mapping,
                          const BroadcastModeSpec& broadcast_spec = BroadcastType::EXPLICIT);

                /// \brief Constructs a broadcast operation.
                ///
                /// \param arg            The input tensor to be broadcast.
                /// \param target_shape   The shape of the output tensor.
                /// \param broadcast_spec Broadcast specification to use for determining broadcast
                ///                       axes
                Broadcast(const Output<Node>& arg,
                          const Output<Node>& target_shape,
                          const BroadcastModeSpec& broadcast_spec = BroadcastType::NUMPY);

                bool visit_attributes(AttributeVisitor& visitor) override;

                std::shared_ptr<Node>
                    clone_with_new_inputs(const OutputVector& new_args) const override;

                // \return Broadcast Specification.
                const BroadcastModeSpec& get_broadcast_spec() const { return m_mode; }
                void set_broadcast_spec(const BroadcastModeSpec& broadcast_spec)
                {
                    m_mode = broadcast_spec;
                }

                void validate_and_infer_types() override;

                /// \return true and the AxisSet if broadcast axes can be fully determined.
                std::pair<bool, AxisSet> get_broadcast_axes() const override;
                bool evaluate(const HostTensorVector& outputs,
                              const HostTensorVector& inputs) const override;
            };
        } // namespace v3

        namespace v1
        {
            /// \brief Operation which "adds" axes to an input tensor, replicating elements from the
            ///        input as needed along the new axes.
            class NGRAPH_API Broadcast : public util::BroadcastBase
            {
            public:
                static constexpr NodeTypeInfo type_info{"Broadcast", 1};
                const NodeTypeInfo& get_type_info() const override { return type_info; }
                /// \brief Constructs a broadcast operation.
                Broadcast() = default;
                /// \brief Constructs a broadcast operation.
                ///
                /// \param arg            The input tensor to be broadcast.
                /// \param target_shape   The shape of the output tensor.
                /// \param axes_mapping   The axis positions (0-based) in the result that correspond
                ///                       to input axes. 'Arg' tensor is broadcast along the
                ///                       remaining axes.
                ///                       E.g., Input Shape - [3, 4], Target Shape - [3, 5, 4, 4]
                ///                       axes_mapping - [0, 2] => Broadcast along axes 1 and 3.
                ///                       axes_mapping - [0, 3] => Broadcast along axes 1 and 2.
                /// \param broadcast_spec Broadcast specification to use for determining broadcast
                ///                       axes. 'axes_mapping' is ignored if broadcast_spec is not
                ///                       NONE
                Broadcast(const Output<Node>& arg,
                          const Output<Node>& target_shape,
                          const Output<Node>& axes_mapping,
                          const AutoBroadcastSpec& broadcast_spec = AutoBroadcastSpec());

                /// \brief Constructs a broadcast operation.
                ///
                /// \param arg            The input tensor to be broadcast.
                /// \param target_shape   The shape of the output tensor.
                /// \param broadcast_spec Broadcast specification to use for determining broadcast
                ///                       axes
                Broadcast(const Output<Node>& arg,
                          const Output<Node>& target_shape,
                          const AutoBroadcastSpec& broadcast_spec =
                              AutoBroadcastSpec(AutoBroadcastType::NUMPY));

                bool visit_attributes(AttributeVisitor& visitor) override;

                std::shared_ptr<Node>
                    clone_with_new_inputs(const OutputVector& new_args) const override;

                /// \return Broadcast Specification.
                const AutoBroadcastSpec& get_broadcast_spec() const { return m_broadcast_spec; }
                void set_broadcast_spec(const AutoBroadcastSpec& broadcast_spec)
                {
                    m_broadcast_spec = broadcast_spec;
                }

                void validate_and_infer_types() override;
                bool evaluate(const HostTensorVector& outputs,
                              const HostTensorVector& inputs) const override;

            protected:
                AutoBroadcastSpec m_broadcast_spec;
            };
        } // namespace v1
    }
}
