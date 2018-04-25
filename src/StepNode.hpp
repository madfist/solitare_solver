#ifndef SOLITARE_SOLVER_STEP_NODE_HEADER
#define SOLITARE_SOLVER_STEP_NODE_HEADER

template<class Step>
class StepNode {
public:
    static int ROOT;
    static int END;
    StepNode(int r, int n, const Step& s) : root_(r), next_(n), step_(s) {}
    int root() { return root_; }
    int next() { return next_; }
    Step& step() { return step_; }
private:
    int root_;
    int next_;
    Step step_;
};

template<class Step> int StepNode<Step>::ROOT = -2;
template<class Step> int StepNode<Step>::END = -1;

#endif