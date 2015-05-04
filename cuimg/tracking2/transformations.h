#ifndef CUIMG_PYRLK_TRACKING_TRANSFORMATIONS_H_
# define CUIMG_PYRLK_TRACKING_TRANSFORMATIONS_H_

# include <eigen3/Eigen/Dense>
# include <cuimg/copy.h>
# include <cuimg/builtin_math.h>

namespace cuimg
{

  struct affine_transform : public Eigen::Matrix<float, 2, 3>
  {
  public:
    typedef Eigen::Matrix<float, 2, 3> super;
    //using super::super;

    inline affine_transform() : super() {}

    template <typename T>
    affine_transform(Eigen::MatrixBase<T> o) : super(o) {}


    static inline affine_transform
    identity() { return super::Identity(); }

    static inline affine_transform
    zero() { return super::Zero(); }

    inline affine_transform
    scale_transform(float factor)
    {
      affine_transform res = *this;
      res.block<2,1>(0,2) *= factor;
      return res;
    }

    inline i_float2
    apply_transform(i_float2 p)
    {
      Eigen::Vector2f res = Eigen::Vector2f(p[0], p[1]) + this->block<2,1>(0,2);
      return i_float2(res[0], res[1]);
    }

  };

  inline float transform_distance(const affine_transform& a, const affine_transform& b)
  {
    return (a.block<2,1>(0,2) - b.block<2,1>(0,2)).norm();
  }

  inline i_float2 transform_velocity(const affine_transform& a)
  {
    Eigen::Vector2f v = a.block<2,1>(0,2);
    return i_float2(v[0], v[1]);
  }


  struct translation_transform : public i_float2
  {
  public:
    typedef i_float2 super;
    //using super::super;

    inline translation_transform() : super() {}

    template <typename T>
    translation_transform(const improved_builtin<T, 2>& t) : super(t) {}
    inline translation_transform(float x, float y) : super(x, y) {}

    static inline translation_transform
    identity() { return i_float2(0,0); }

    static inline translation_transform
    zero() { return i_float2(0,0); }

    inline translation_transform
    scale_transform(float factor)
    {
      return 2 * *this;
    }

    inline i_float2
    apply_transform(i_float2 p)
    {
      return p + *this;
    }

  };

  inline float transform_distance(const translation_transform& a, const translation_transform& b)
  {
    return norml2(a - b);
  }

  inline i_float2 transform_velocity(const translation_transform& a)
  {
    return a;
  }

}

#endif
