/*---------------------------------------------------------------------------------*/
/*  NOMAD - Nonlinear Optimization by Mesh Adaptive Direct Search -                */
/*                                                                                 */
/*  NOMAD - Version 4 has been created by                                          */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  The copyright of NOMAD - version 4 is owned by                                 */
/*                 Charles Audet               - Polytechnique Montreal            */
/*                 Sebastien Le Digabel        - Polytechnique Montreal            */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  NOMAD 4 has been funded by Rio Tinto, Hydro-Québec, Huawei-Canada,             */
/*  NSERC (Natural Sciences and Engineering Research Council of Canada),           */
/*  InnovÉÉ (Innovation en Énergie Électrique) and IVADO (The Institute            */
/*  for Data Valorization)                                                         */
/*                                                                                 */
/*  NOMAD v3 was created and developed by Charles Audet, Sebastien Le Digabel,     */
/*  Christophe Tribes and Viviane Rochon Montplaisir and was funded by AFOSR       */
/*  and Exxon Mobil.                                                               */
/*                                                                                 */
/*  NOMAD v1 and v2 were created and developed by Mark Abramson, Charles Audet,    */
/*  Gilles Couture, and John E. Dennis Jr., and were funded by AFOSR and           */
/*  Exxon Mobil.                                                                   */
/*                                                                                 */
/*  Contact information:                                                           */
/*    Polytechnique Montreal - GERAD                                               */
/*    C.P. 6079, Succ. Centre-ville, Montreal (Quebec) H3C 3A7 Canada              */
/*    e-mail: nomad@gerad.ca                                                       */
/*                                                                                 */
/*  This program is free software: you can redistribute it and/or modify it        */
/*  under the terms of the GNU Lesser General Public License as published by       */
/*  the Free Software Foundation, either version 3 of the License, or (at your     */
/*  option) any later version.                                                     */
/*                                                                                 */
/*  This program is distributed in the hope that it will be useful, but WITHOUT    */
/*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or          */
/*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License    */
/*  for more details.                                                              */
/*                                                                                 */
/*  You should have received a copy of the GNU Lesser General Public License       */
/*  along with this program. If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                                 */
/*  You can find information on the NOMAD software at www.gerad.ca/nomad           */
/*---------------------------------------------------------------------------------*/

#include "../../Algos/Mads/OrthoNPlus1NegPollMethod.hpp"
#include "../../Math/Direction.hpp"


void NOMAD::OrthoNPlus1NegPollMethod::init()
{
    setStepType(NOMAD::StepType::POLL_METHOD_ORTHO_NPLUS1_NEG);
    verifyParentNotNull();

}

// Generate poll directions
void NOMAD::OrthoNPlus1NegPollMethod::generateUnitPollDirections(std::list<NOMAD::Direction> &directions, const size_t n) const
{
    directions.clear();

    generate2NDirections(directions, n);
}


// Ref in NOMAD 3: NOMAD::Mads::get_single_dynamic_direction.
// Get a single dynamic direction from incomplete poll by sum of negative
void NOMAD::OrthoNPlus1NegPollMethod::generateNPlus1Direction(std::list<Direction> &directions) const
{
    // Gather directions for points which are already generated - which are currently
    // in _trialPoints.
    NOMAD::Direction dirNPlus1;
    for (auto trialPoint : _trialPoints)
    {
        auto dir = trialPoint.getDirection();
        if (nullptr != dir)
        {
            if (0 == dirNPlus1.size())
            {
                dirNPlus1 = -(*dir);
            }
            else
            {
                dirNPlus1 -= (*dir);
            }
        }
    }
    if (dirNPlus1.size() > 0 && dirNPlus1.norm() > 0 && _trialPoints.size() > 0)
    {
        dirNPlus1 /= _trialPoints.size();
        directions.push_back(dirNPlus1);
    }
}

