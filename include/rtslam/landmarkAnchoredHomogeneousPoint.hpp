/**
 * \file landmarkAnchoredHomogeneousPoint.hpp
 * \author jsola@laas.fr
 * \date 14/02/2010
 *
 * Header file for anchored homogeneous point
 * \ingroup rtslam
 */

#ifndef LANDMARKANCHOREDHOMOGENEOUSPOINT_HPP_
#define LANDMARKANCHOREDHOMOGENEOUSPOINT_HPP_

#include "boost/shared_ptr.hpp"
#include "rtslam/landmarkAbstract.hpp"
#include "rtslam/quatTools.hpp"
#include "rtslam/ahpTools.hpp"

/**
 * General namespace for Jafar environment.
 * \ingroup jafar
 */
namespace jafar {
	namespace rtslam {

		class LandmarkAnchoredHomogeneousPoint;
		typedef boost::shared_ptr<LandmarkAnchoredHomogeneousPoint> ahp_ptr_t;



		/**
		 * Class for anchored homogeneous 3D points
		 * \ingroup rtslam
		 */
		class LandmarkAnchoredHomogeneousPoint: public LandmarkAbstract {
			public:


				/**
				 * Constructor from map
				 */
				LandmarkAnchoredHomogeneousPoint(const map_ptr_t & mapPtr);

				static size_t size(void) {
					return 7;
				}


				/**
				 * From-frame transform.
				 * \param F a frame to transform from
				 * \param ahpf an AHP point in F-frame
				 * \return the AHP point in global frame
				 */
				template<class VF>
				vec fromFrame(const VF & F) {
					return landmarkAHP::fromFrame(F, state.x());
				}


				/**
				 * From-frame transform, with Jacobians.
				 * \param F a frame to transform from
				 * \param ahpf an AHP point in F-frame
				 * \param ahp the AHP point in global frame
				 * \param AHP_f the Jacobian of \a ahp wrt \a F
				 * \param AHP_ahpf the Jacobians of \a ahp wrt \a ahpf
				 */
				template<class VF, class Vahp, class MAHP_f, class MAHP_ahpf>
				void fromFrame(const VF & F, Vahp & ahp, MAHP_f & AHP_f, MAHP_ahpf & AHP_ahpf) {
					landmarkAHP::fromFrame(F, state.x(), ahp, AHP_f, AHP_ahpf);
				}


				/**
				 * To-frame transform.
				 * \param F a frame to transform to
				 * \param ahp an AHP point in global frame
				 * \return the AHP point in F-frame
				 */
				template<class VF>
				vec toFrame(const VF & F) {
					return landmarkAHP::toFrame(F, state.x());
				}


				/**
				 * To-frame transform, with Jacobians.
				 * \param F a frame to transform to
				 * \param ahp an AHP point in global frame
				 * \param ahpf the AHP point in F-frame
				 * \param AHPF_f the Jacobian of \a ahpf wrt \a F
				 * \param AHPF_ahp the Jacobians of \a ahpf wrt \a ahp
				 */
				template<class VF, class Vahpf, class MAHPF_f, class MAHPF_ahp>
				void toFrame(const VF & F, Vahpf & ahpf, MAHPF_f & AHPF_f, MAHPF_ahp & AHPF_ahp) {
					landmarkAHP::toFrame(F, state.x(), ahpf, AHPF_f, AHPF_ahp);
				}


				/**
				 * Reparametrize to Euclidean.
				 * \param ahp the anchored homogeneous point to be reparametrized.
				 * \return the Euclidean point.
				 */
				vec3 toEuclidean();


				/**
				 * Reparametrize to Euclidean, with Jacobians.
				 * \param ahp the anchored homogeneous point to be reparametrized.
				 * \param euc the returned Euclidean point.
				 * \param EUC_ahp the Jacobian of the conversion.
				 */
				template<class VE, class ME_a>
				void toEuclidean(VE & euc, ME_a & EUC_ahp) {
					landmarkAHP::ahp2euc(state.x(), euc, EUC_ahp);
				}

				/**
				 * Bring landmark to bearing-only sensor frame (without range information).
				 *
				 * For a landmark and sensor frame
				 * - ahp = [p0 m rho]
				 * - s = [t q],
				 *
				 * this function computes the chain (See Sola \e etal. PAMI 2010):
				 * - R'(q) * ( m - (t - p0) * rho )
				 *
				 * which is a vector in sensor frame in the direction of the landmark. The range information is lost.
				 *
				 * \param s the sensor frame
				 * \param ahp the AHP landmark
				 * \return the bearing-only landmark in sensor frame
				 */
				template<class VS>
				vec3 toBearingOnlyFrame(const VS & s) {
						return landmarkAHP::toBearingOnlyFrame(s, state.x());
				}

				/**
				 * Bring landmark to bearing-only sensor frame (give inverse-distance information).
				 *
				 * For a landmark and sensor frame
				 * - ahp = [p0 m rho]
				 * - s = [t q],
				 *
				 * this function computes the chain (See Sola \e etal. PAMI 2010):
				 * - v = R'(q) * ( m - (t - p0) * rho )
				 *
				 * which is a vector in sensor frame in the direction of the landmark.
				 *
				 * The range information is recuperated in \a invDist as the inverse of the distance from sensor to landmark.
				 *
				 * \param s the sensor frame
				 * \param ahp the AHP landmark
				 * \param v the bearing-only landmark in sensor frame
				 * \param invDist the inverse of the non-observable distance
				 */
				template<class VS, class VV>
				void toBearingOnlyFrame(const VS & s, VV & v, double & invDist) {
						landmarkAHP::toBearingOnlyFrame(s, state.x(), v, invDist);
				}

				/**
				 * Bring landmark to bearing-only sensor frame (give inverse-distance information).
				 *
				 * For a landmark and sensor frame
				 * - ahp = [p0 m rho]
				 * - s = [t q],
				 *
				 * this function computes the chain (See Sola \e etal. PAMI 2010):
				 * - R'(q) * ( m - (t - p0) * rho )
				 *
				 * which is a vector in sensor frame in the direction of the landmark.
				 *
				 * The range information is recuperated in \a invDist as the inverse of the distance from sensor to landmark.
				 *
				 * and returns the Jacobians wrt s and ahp.
				 * \param s the sensor frame
				 * \param ahp the AHP landmark
				 * \param v the bearing-only landmark in sensor frame
				 * \param V_s the Jacobian of \a v wrt \a s
				 * \param V_ahp the Jacobian of \a v wrt \a ahp
				 */
				template<class VS, class VV, class MV_s, class MV_a>
				void toBearingOnlyFrame(const VS & s, VV & v, double & invDist, MV_s & V_s, MV_a & V_ahp) {
						landmarkAHP::toBearingOnlyFrame(s, state.x(), v, invDist, V_s, V_ahp);
				}

				/**
				 * AHP landmark from bearing-only retro-projection.
				 * This function is the inverse of toBearingOnlyFrame(). It builds the Anchored Homogeneous Point (AHP) landmark from a
				 * sensor frame \a s, a retro-projected director vector \a v, and a inverse-distance proportional prior \a rho.
				 *
				 * It uses the formula (See Sola \e etal. PAMI 2010):
				 * - AHP = [ t ;  R(q) * v ; rho * norm(v) ]
				 *
				 * so that \a rho can be specified as being exactly inverse-distance.
				 *
				 * \param s the sensor frame
				 * \param v the retro-projected director vector in sensor frame
				 * \param rho the prior, proportional to inverse-distance
				 * \return the AHP landmark.
				 */
				template<class VS, class VLS>
				static vec7 fromBearingOnlyFrame(const VS & s, const VLS & v, const double _rho) {
						return landmarkAHP::fromBearingOnlyFrame(s, v, _rho);
				}

				/**
				 * AHP landmark from bearing-only retro-projection, with Jacobians.
				 * This function is the inverse of toBearingOnlyFrame(). It builds the Anchored Homogeneous Point (AHP) landmark from a
				 * sensor frame \a s, a retro-projected director vector \a v, and a inverse-distance proportional prior \a rho.
				 *
				 * It uses the formula (See Sola \e etal. PAMI 2010):
				 * - AHP = [ t ;  R(q) * v ; rho * norm(v) ]
				 *
				 * so that \a rho can be specified as being exactly inverse-distance.
				 *
				 * \param s the sensor frame
				 * \param v the retro-projected director vector in sensor frame
				 * \param rho the prior, proportional to inverse-distance
				 * \param ahp the AHP landmark.
				 * \param AHP_s the Jacobian wrt \a s
				 * \param AHP_v the Jacobian wrt \a v
				 * \param AHP_rho the Jacobian wrt \a rho
				 */
				template<class VS, class VLS, class VA, class MA_s, class MA_v, class MA_rho>
				static void fromBearingOnlyFrame(const VS & s, const VLS & v, const double _rho, VA & ahp, MA_s & AHP_s, MA_v & AHP_v, MA_rho & AHP_rho) {
						landmarkAHP::fromBearingOnlyFrame(s, v, _rho, ahp, AHP_s, AHP_v, AHP_rho);
				}


		}; // class LandmarkAnchoredHomogeneousPoint


	} // namespace rtslam
} // namespace jafar

#endif /* LANDMARKANCHOREDHOMOGENEOUSPOINT_HPP_ */
