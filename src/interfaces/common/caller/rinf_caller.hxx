#ifndef RINF_CALLER_HXX_
#define RINF_CALLER_HXX_

#include <opengm/opengm.hxx>
#include <opengm/inference/reducedinference.hxx>


#ifdef WITH_CPLEX
#include <opengm/inference/lpcplex.hxx>
#include <opengm/inference/multicut.hxx>
#endif
#ifdef WITH_GUROBI
#include <opengm/inference/lpgurobi.hxx>
#endif
#ifdef WITH_FASTPD
#include <opengm/inference/external/fastPD.hxx>
#endif
#ifdef WITH_TRWS
#include <opengm/inference/external/trws.hxx>
#endif
#ifdef WITH_GCO
#include <opengm/inference/external/gco.hxx>
#endif
#ifdef WITH_MPLP
#include <opengm/inference/external/mplp.hxx>
#endif

#include "inference_caller_base.hxx"
#include "../argument/argument.hxx"

namespace opengm {

namespace interface {

template <class IO, class GM, class ACC>
class RINFCaller : public InferenceCallerBase<IO, GM, ACC, RINFCaller<IO, GM, ACC> > {
protected:

   typedef InferenceCallerBase<IO, GM, ACC, RINFCaller<IO, GM, ACC> > BaseClass;
   typedef typename BaseClass::OutputBase OutputBase;
   using BaseClass::addArgument;
   using BaseClass::io_;
   using BaseClass::infer; 

   virtual void runImpl(GM& model, OutputBase& output, const bool verbose);

   bool persistency_;
   bool tentacle_;
   bool connectedComponents_;
   double timeOut_;
   int numberOfThreads_;
   std::string selectedInfType_;
   
public:
   const static std::string name_;
   RINFCaller(IO& ioIn);
   ~RINFCaller();
};

template <class IO, class GM, class ACC>
inline RINFCaller<IO, GM, ACC>::RINFCaller(IO& ioIn)
: BaseClass(name_, "detailed description of RINF caller...", ioIn) {
   std::vector<std::string> inf;
   inf.push_back("ILP"); 
   inf.push_back("ILP-GUROBI");
   inf.push_back("LP");
   inf.push_back("MC");
   inf.push_back("MCR");
   inf.push_back("MCR-LP");
   inf.push_back("MCR-CP"); 
   inf.push_back("MCR-CP2");       
   inf.push_back("TRWS");
   inf.push_back("FASTPD"); 
   inf.push_back("EXPANSION"); 
   inf.push_back("MPLP-C");    
   addArgument(StringArgument<>(selectedInfType_, "i", "inf", "Select inference method for reduced problems.", inf.front(), inf));
   addArgument(IntArgument<>(numberOfThreads_, "", "threads", "number of threads", static_cast<int>(1)));
   addArgument(BoolArgument(persistency_, "ps", "persistency", "use reduction persistency"));
   addArgument(BoolArgument(tentacle_, "t", "tentacle", "use reduction by removing tentacles"));
   addArgument(BoolArgument(connectedComponents_, "c", "conectedcomp", "use reduction by finding connect components"));
   double to =14400.0;
   addArgument(DoubleArgument<>(timeOut_,"","timeout","maximal runtime in seconds",to));
}

template <class IO, class GM, class ACC>
RINFCaller<IO, GM, ACC>::~RINFCaller()
{;}

template <class IO, class GM, class ACC>
inline void RINFCaller<IO, GM, ACC>::runImpl(GM& model, OutputBase& output, const bool verbose) {
   std::cout << "running RINF caller" << std::endl;


   if(selectedInfType_=="ILP"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef LPCplex<GM2, ACC> LPCPLEX;
      typedef ReducedInference<GM,ACC,LPCPLEX> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType; 

      typename RINF::Parameter rinfParameter_;  

      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.timeLimit_         = timeOut_;
      rinfParameter_.subParameter_.numberOfThreads_   = numberOfThreads_;
      rinfParameter_.subParameter_.integerConstraint_ = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("CPLEX is disabled!");
#endif
   } 
   else if(selectedInfType_=="ILP-GUROBI"){
#ifdef WITH_GUROBI
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef LPGurobi<GM2, ACC> LPCPLEX;
      typedef ReducedInference<GM,ACC,LPCPLEX> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType; 

      typename RINF::Parameter rinfParameter_;  

      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.timeLimit_         = timeOut_;
      rinfParameter_.subParameter_.numberOfThreads_   = numberOfThreads_;
      rinfParameter_.subParameter_.integerConstraint_ = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("GUROBI is disabled!");
#endif
   } 
   else if(selectedInfType_=="LP"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef LPCplex<GM2, ACC> LPCPLEX;
      typedef ReducedInference<GM,ACC,LPCPLEX> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType; 

      typename RINF::Parameter rinfParameter_;  

      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.timeLimit_         = timeOut_;
      rinfParameter_.subParameter_.numberOfThreads_   = numberOfThreads_;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("CPLEX is disabled!");
#endif
   }
   else if(selectedInfType_=="MC"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef Multicut<GM2, ACC> MultiCut;
      typedef ReducedInference<GM,ACC,MultiCut> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_;
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.numThreads_ = numberOfThreads_;
      rinfParameter_.subParameter_.timeOut_    = timeOut_;
      rinfParameter_.subParameter_.workFlow_   = "(TTC)(MTC)(IC)(TTC-I)"; 
      rinfParameter_.subParameter_.useBufferedStates_   = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MULTICUT is disabled!");
#endif
   }   
   else if(selectedInfType_=="MCR"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef Multicut<GM2, ACC> MultiCut;
      typedef ReducedInference<GM,ACC,MultiCut> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_;
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.numThreads_ = numberOfThreads_;
      rinfParameter_.subParameter_.timeOut_    = timeOut_;
      rinfParameter_.subParameter_.workFlow_   = "(TTC)(MTC)";
      rinfParameter_.subParameter_.useBufferedStates_   = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MULTICUT is disabled!");
#endif
   } 
   else if(selectedInfType_=="MCR-LP"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef Multicut<GM2, ACC> MultiCut;
      typedef ReducedInference<GM,ACC,MultiCut> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_;
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.numThreads_ = numberOfThreads_;
      rinfParameter_.subParameter_.timeOut_    = timeOut_;
      rinfParameter_.subParameter_.workFlow_   = "(TTC)(TTC,MTC)"; 
      rinfParameter_.subParameter_.useBufferedStates_   = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MULTICUT is disabled!");
#endif
   } 
   else if(selectedInfType_=="MCR-CP"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef Multicut<GM2, ACC> MultiCut;
      typedef ReducedInference<GM,ACC,MultiCut> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_;
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.numThreads_ = numberOfThreads_;
      rinfParameter_.subParameter_.timeOut_    = timeOut_;
      rinfParameter_.subParameter_.workFlow_   = "(TTC)(TTC,MTC)(TTC,MTC,CC-FDB)";
      rinfParameter_.subParameter_.useBufferedStates_   = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MULTICUT is disabled!");
#endif
   }  
   else if(selectedInfType_=="MCR-CP2"){
#ifdef WITH_CPLEX
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef Multicut<GM2, ACC> MultiCut;
      typedef ReducedInference<GM,ACC,MultiCut> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_;
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;

      rinfParameter_.subParameter_.numThreads_ = numberOfThreads_;
      rinfParameter_.subParameter_.timeOut_    = timeOut_;
      rinfParameter_.subParameter_.workFlow_   = "(TTC)(MTC)(TTC,CC)";
      rinfParameter_.subParameter_.useBufferedStates_   = true;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MULTICUT is disabled!");
#endif
   }   
   else if(selectedInfType_=="TRWS"){
#ifdef WITH_TRWS
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef typename opengm::external::TRWS<GM2> TRWSType;
      typedef ReducedInference<GM,ACC,TRWSType> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_; 
  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;
  

      rinfParameter_.subParameter_.energyType_= TRWSType::Parameter::VIEW;
      rinfParameter_.subParameter_.numberOfIterations_ = 1000;
      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("TRWS is disabled!");
#endif
   }
   else if(selectedInfType_=="FASTPD"){
#ifdef WITH_FASTPD
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef typename opengm::external::FastPD<GM2> FastPDType;
      typedef ReducedInference<GM,ACC,FastPDType> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_; 

  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;
  
      rinfParameter_.subParameter_.numberOfIterations_ = 1000;

      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("FASTPD is disabled!");
#endif
   }
   else if(selectedInfType_=="EXPANSION"){
#ifdef WITH_GCO
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2;
      typedef typename opengm::external::GCOLIB<GM2> GCOLIB;
      typedef ReducedInference<GM,ACC,GCOLIB> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_; 

  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;
   
      rinfParameter_.subParameter_.inferenceType_= GCOLIB::Parameter::EXPANSION;   
      rinfParameter_.subParameter_.energyType_= GCOLIB::Parameter::VIEW;
      rinfParameter_.subParameter_.doNotUseGrid_= true;
      rinfParameter_.subParameter_.numberOfIterations_ = 1000;

      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("GCOLIB is disabled!");
#endif
   }
   else if(selectedInfType_=="MPLP-C"){
#ifdef WITH_MPLP
      std::cout<<" start MPLP"<<std::endl;
      typedef typename ReducedInferenceHelper<GM>::InfGmType GM2; 
      typedef typename opengm::external::MPLP<GM2> MPLP;
      typedef ReducedInference<GM,ACC,MPLP> RINF;

      typedef typename RINF::VerboseVisitorType VerboseVisitorType;
      typedef typename RINF::EmptyVisitorType EmptyVisitorType;
      typedef typename RINF::TimingVisitorType TimingVisitorType;

      typename RINF::Parameter rinfParameter_; 

  
      rinfParameter_.Persistency_         = persistency_;
      rinfParameter_.Tentacle_            = tentacle_;
      rinfParameter_.ConnectedComponents_ = connectedComponents_;
   
      rinfParameter_.subParameter_.maxTime_= timeOut_;   
      rinfParameter_.subParameter_.maxTimeLP_= timeOut_/3.0;
      rinfParameter_.subParameter_.maxIterLP_ = 10000;
      rinfParameter_.subParameter_.maxIterTight_ = 10000;

      this-> template infer<RINF, TimingVisitorType, typename RINF::Parameter>(model, output, verbose, rinfParameter_);
#else
      throw RuntimeError("MPLP is disabled!");
#endif
   }
   else{
      throw RuntimeError("Unknown Inference method for subproblems!");
   }
}

template <class IO, class GM, class ACC>
const std::string RINFCaller<IO, GM, ACC>::name_ = "RINF";

} // namespace interface

} // namespace opengm

#endif /* RINF_CALLER_HXX_ */
