/*---------------------------------------------------------------------------------*/
/*  NOMAD - Nonlinear Optimization by Mesh Adaptive Direct Search -                */
/*                                                                                 */
/*  NOMAD - Version 4.0.0 has been created by                                      */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  The copyright of NOMAD - version 4.0.0 is owned by                             */
/*                 Charles Audet               - Polytechnique Montreal            */
/*                 Sebastien Le Digabel        - Polytechnique Montreal            */
/*                 Viviane Rochon Montplaisir  - Polytechnique Montreal            */
/*                 Christophe Tribes           - Polytechnique Montreal            */
/*                                                                                 */
/*  NOMAD v4 has been funded by Rio Tinto, Hydro-Québec, NSERC (Natural            */
/*  Sciences and Engineering Research Council of Canada), InnovÉÉ (Innovation      */
/*  en Énergie Électrique) and IVADO (The Institute for Data Valorization)         */
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
/*    phone : 1-514-340-6053 #6928                                                 */
/*    fax   : 1-514-340-5665                                                       */
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

#include "../../Algos/AlgoStopReasons.hpp"
#include "../../Algos/Mads/NMSearchMethod.hpp"
#include "../../Algos/Mads/MadsIteration.hpp"
#include "../../Algos/EvcInterface.hpp"
#include "../../Algos/NelderMead/NM.hpp"
#include "../../Algos/NelderMead/NMAllReflective.hpp"

void NOMAD::NMSearchMethod::init()
{
    if ( _runParams->getAttributeValue<bool>("GENERATE_ALL_POINTS_BEFORE_EVAL") )
    {
        _name = "Search (Nelder Mead single pass)";
    }
    else
    {
        _name = "Search (Nelder Mead optimization)";
    }
    //setComment("(NMSearch)");

    auto nmSearch = _runParams->getAttributeValue<bool>("NM_SEARCH");
    setEnabled(nmSearch);

    if (nmSearch)
    {
        // Set the lap counter
        auto nmFactor = _runParams->getAttributeValue<size_t>("NM_SEARCH_MAX_TRIAL_PTS_NFACTOR");
        auto dim = _pbParams->getAttributeValue<size_t>("DIMENSION");
        if (nmFactor < NOMAD::INF_SIZE_T)
        {
            NOMAD::EvcInterface::getEvaluatorControl()->setLapMaxBbEval( dim*nmFactor );
        }
    }
}


bool NOMAD::NMSearchMethod::runImp()
{
    // NM is an algorithm with its own stop reasons.
    auto nmStopReasons = std::make_shared<NOMAD::AlgoStopReasons<NOMAD::NMStopType>>();

    // Create the NM algorithm with its own stop reason
    auto nm = std::make_shared<NOMAD::NM>(this,
                                          nmStopReasons ,
                                          _runParams,
                                          _pbParams);
    nm->setEndDisplay(false);

    nm->start();
    bool foundBetter = nm->run();
    nm->end();
    
    

    return foundBetter;
}


void NOMAD::NMSearchMethod::generateTrialPointsImp()
{
    // The trial points of one iteration of NM reflective steps are generated (not evaluated).
    // The trial points are Reflect, Expansion, Inside and Outside Contraction NM points

    auto madsIteration = getParentOfType<MadsIteration*>();

    NOMAD::NMAllReflective allReflective(this, madsIteration->getFrameCenter(), madsIteration->getMesh());
    allReflective.start();
    allReflective.end();

    // Pass the generated trial pts to this
    auto trialPtsNM = allReflective.getTrialPoints();
    for (auto point : trialPtsNM)
    {
        insertTrialPoint(point);
    }

}
