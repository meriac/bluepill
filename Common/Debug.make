#  ##################################################################
#  # @file     Flashing & SWO Debug Helper Makefile
#  # @brief    Demonstrate JLink SWO debug printing and flashing
#  # @date     17. April 2020
#  ##################################################################
#
#  Copyright (c) 2020 Milosch Meriac <milosch@meriac.com>.
#  All rights reserved.
#
#  SPDX-License-Identifier: Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the License);
#  You may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an AS IS BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
#  or implied. See the License for the specific language governing
#  permissions and limitations under the License.
#

.PHONY: flash

flash: $(PROJECT).bin
	st-flash write $^ 8000000

