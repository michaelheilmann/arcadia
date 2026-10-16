// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Media/Include.h"
#include "Arcadia/ADL/Include.h"

void
Arcadia_Media_PixelBufferOperations_requireDefinitionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* definition
  )
{
  if (!definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Media_PixelBufferOperations_Fill_applyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  if (NULL == self->definition->color->definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_ColorDefinition* color = (Arcadia_ADL_ColorDefinition*)self->definition->color->definition;
  Arcadia_Natural8Value red = color->red;
  Arcadia_Natural8Value green = color->green;
  Arcadia_Natural8Value blue = color->blue;
  Arcadia_Media_PixelBuffer_fillRectangle(thread, target, 0, 0, target->width, target->height, red, green, blue, 255);
}

void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_applyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  if (0 == self->definition->checkerWidth || 0 == self->definition->checkerHeight) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (NULL == self->definition->firstCheckerColor->definition || NULL == self->definition->secondCheckerColor->definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_ColorDefinition* firstCheckerColor = (Arcadia_ADL_ColorDefinition*)self->definition->firstCheckerColor->definition,
                             * secondCheckerColor = (Arcadia_ADL_ColorDefinition*)self->definition->secondCheckerColor->definition;
  Arcadia_Integer32Value checkerWidth = self->definition->checkerWidth,
                         checkerHeight = self->definition->checkerHeight;
  for (Arcadia_Integer32Value y = 0; y < Arcadia_Media_PixelBuffer_getHeight(thread, target) / checkerHeight + 1; ++y) {
    for (Arcadia_Integer32Value x = 0; x < Arcadia_Media_PixelBuffer_getWidth(thread, target) / checkerWidth + 1; ++x) {
      if ((x + y) % 2 == 0) {
        Arcadia_Natural8Value red = firstCheckerColor->red;
        Arcadia_Natural8Value green = firstCheckerColor->green;
        Arcadia_Natural8Value blue = firstCheckerColor->blue;
        Arcadia_Media_PixelBuffer_fillRectangle(thread, target, x * checkerWidth, y * checkerHeight, checkerWidth, checkerHeight,
                                                  red, green, blue, 255);
      } else {
        Arcadia_Natural8Value red = secondCheckerColor->red;
        Arcadia_Natural8Value green = secondCheckerColor->green;
        Arcadia_Natural8Value blue = secondCheckerColor->blue;
        Arcadia_Media_PixelBuffer_fillRectangle(thread, target, x * checkerWidth, y * checkerHeight, checkerWidth, checkerHeight,
                                                  red, green, blue, 255);
      }

    }
  }
}
