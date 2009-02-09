
%{
#include "renderable.h"
%}

%ignore Renderable::collides_with(const Ray &ray, double &t);
%ignore Renderable::initialize(xmlNode * node);

%include "renderable.h"
