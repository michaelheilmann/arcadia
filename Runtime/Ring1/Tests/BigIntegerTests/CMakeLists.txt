# Arcadia
# Copyright (C) 2024-2026 Michael Heilmann
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.29)

set(this ${MyProjectName}.Ring1.Tests.BigIntegerTests)

# Create test executable.
BeginProduct(${this} test)

SetFolder(${this} "Runtime/Ring1")

OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Additive.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Additive.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/And.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/And.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Main.c)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Multiplicative.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Multiplicative.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Or.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Or.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/PowersOf.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/PowersOf.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/QuotientRemainder.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/QuotientRemainder.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Relational.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Relational.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/SetInteger.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/SetInteger.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Shift.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/Shift.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/TwosComplement.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/TwosComplement.h)
OnSourceFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/DecimalString.c)
OnHeaderFile(${this} Arcadia.Ring1.Tests.BigIntegerTests/DecimalString.h)

OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)

EndProduct(${this})
