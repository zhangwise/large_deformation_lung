#include "poro.h"
#include "poro_elastic_cc.h"

void assemble_postvars (EquationSystems& es,
                      const std::string& system_name, Tree& tree)
{

   Real E    = es.parameters.get<Real>("E");
   Real NU    = es.parameters.get<Real>("NU");
   //Real KPERM    = es.parameters.get<Real>("KPERM");
  
	Real sum_jac_postvars=0;
	  System& aux_system = es.get_system<System>("Reference-Configuration");

#include "assemble_preamble_postvars.cpp"

  for ( ; el != end_el; ++el)
    {    
 
      const Elem* elem = *el;

      dof_map.dof_indices (elem, dof_indices);
      dof_map.dof_indices (elem, dof_indices_u, u_var);
      dof_map.dof_indices (elem, dof_indices_v, v_var);
      dof_map.dof_indices (elem, dof_indices_p, p_var);
      dof_map.dof_indices (elem, dof_indices_x, x_var);
      dof_map.dof_indices (elem, dof_indices_y, y_var);
      dof_map.dof_indices (elem, dof_indices_w, w_var);
      dof_map.dof_indices (elem, dof_indices_z, z_var);
      

      const unsigned int n_dofs   = dof_indices.size();
      const unsigned int n_u_dofs = dof_indices_u.size(); 
      const unsigned int n_v_dofs = dof_indices_v.size();
      const unsigned int n_p_dofs = dof_indices_p.size();
      const unsigned int n_x_dofs = dof_indices_x.size(); 
      const unsigned int n_y_dofs = dof_indices_y.size();
      const unsigned int n_w_dofs = dof_indices_w.size();
      const unsigned int n_z_dofs = dof_indices_z.size();
      
      fe_disp->reinit  (elem);
      fe_vel->reinit  (elem);
      fe_pres->reinit (elem);

      Ke.resize (n_dofs, n_dofs);
      Fe.resize (n_dofs);

      Kuu.reposition (u_var*n_u_dofs, u_var*n_u_dofs, n_u_dofs, n_u_dofs);
      Kuv.reposition (u_var*n_u_dofs, v_var*n_u_dofs, n_u_dofs, n_v_dofs);
      Kup.reposition (u_var*n_u_dofs, p_var*n_u_dofs, n_u_dofs, n_p_dofs);
      Kux.reposition (u_var*n_u_dofs, p_var*n_u_dofs + n_p_dofs , n_u_dofs, n_x_dofs);
      Kuy.reposition (u_var*n_u_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_u_dofs, n_y_dofs);
      Kuw.reposition (u_var*n_u_dofs, w_var*n_u_dofs, n_u_dofs, n_w_dofs);
      Kuz.reposition (u_var*n_u_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_u_dofs, n_z_dofs);
      

      Kvu.reposition (v_var*n_v_dofs, u_var*n_v_dofs, n_v_dofs, n_u_dofs);
      Kvv.reposition (v_var*n_v_dofs, v_var*n_v_dofs, n_v_dofs, n_v_dofs);
      Kvp.reposition (v_var*n_v_dofs, p_var*n_v_dofs, n_v_dofs, n_p_dofs);
      Kvx.reposition (v_var*n_v_dofs, p_var*n_u_dofs + n_p_dofs , n_v_dofs, n_x_dofs);
      Kvy.reposition (v_var*n_v_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_v_dofs, n_y_dofs);
      Kvw.reposition (v_var*n_u_dofs, w_var*n_u_dofs, n_v_dofs, n_w_dofs);
      Kuz.reposition (v_var*n_u_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_u_dofs, n_z_dofs);

      Kwu.reposition (w_var*n_w_dofs, u_var*n_v_dofs, n_v_dofs, n_u_dofs);
      Kwv.reposition (w_var*n_w_dofs, v_var*n_v_dofs, n_v_dofs, n_v_dofs);
      Kwp.reposition (w_var*n_w_dofs, p_var*n_v_dofs, n_v_dofs, n_p_dofs);
      Kwx.reposition (w_var*n_w_dofs, p_var*n_u_dofs + n_p_dofs , n_v_dofs, n_x_dofs);
      Kwy.reposition (w_var*n_w_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_v_dofs, n_y_dofs);
      Kww.reposition (w_var*n_w_dofs, w_var*n_u_dofs, n_v_dofs, n_w_dofs);
      Kwz.reposition (w_var*n_w_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_u_dofs, n_z_dofs);

      Kpu.reposition (p_var*n_u_dofs, u_var*n_u_dofs, n_p_dofs, n_u_dofs);
      Kpv.reposition (p_var*n_u_dofs, v_var*n_u_dofs, n_p_dofs, n_v_dofs);
      Kpp.reposition (p_var*n_u_dofs, p_var*n_u_dofs, n_p_dofs, n_p_dofs);
      Kpx.reposition (p_var*n_v_dofs, p_var*n_u_dofs + n_p_dofs , n_p_dofs, n_x_dofs);
      Kpy.reposition (p_var*n_v_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_p_dofs, n_y_dofs);
      Kpw.reposition (p_var*n_u_dofs, w_var*n_u_dofs, n_p_dofs, n_w_dofs);
      Kpz.reposition (p_var*n_u_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_p_dofs, n_z_dofs);

      Kxu.reposition (p_var*n_u_dofs + n_p_dofs, u_var*n_u_dofs, n_x_dofs, n_u_dofs);
      Kxv.reposition (p_var*n_u_dofs + n_p_dofs, v_var*n_u_dofs, n_x_dofs, n_v_dofs);
      Kxp.reposition (p_var*n_u_dofs + n_p_dofs, p_var*n_u_dofs, n_x_dofs, n_p_dofs);
      Kxx.reposition (p_var*n_u_dofs + n_p_dofs, p_var*n_u_dofs + n_p_dofs , n_x_dofs, n_x_dofs);
      Kxy.reposition (p_var*n_u_dofs + n_p_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_x_dofs, n_y_dofs);
      Kxw.reposition (p_var*n_u_dofs + n_p_dofs, w_var*n_u_dofs, n_x_dofs, n_w_dofs);
      Kxz.reposition (p_var*n_u_dofs + n_p_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_x_dofs, n_z_dofs);


      Kyu.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, u_var*n_u_dofs, n_y_dofs, n_u_dofs);
      Kyv.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, v_var*n_u_dofs, n_y_dofs, n_v_dofs);
      Kyp.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, p_var*n_u_dofs, n_y_dofs, n_p_dofs);
      Kyx.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, p_var*n_u_dofs + n_p_dofs , n_y_dofs, n_x_dofs);
      Kyy.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_y_dofs, n_y_dofs);
      Kyw.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, w_var*n_u_dofs, n_x_dofs, n_w_dofs);
      Kyz.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_x_dofs, n_z_dofs);

      Kzu.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, u_var*n_u_dofs, n_y_dofs, n_u_dofs);
      Kzv.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, v_var*n_u_dofs, n_y_dofs, n_v_dofs);
      Kzp.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, p_var*n_u_dofs, n_y_dofs, n_p_dofs);
      Kzx.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, p_var*n_u_dofs + n_p_dofs , n_y_dofs, n_x_dofs);
      Kzy.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, p_var*n_u_dofs + n_p_dofs+n_x_dofs , n_y_dofs, n_y_dofs);
      Kzw.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, w_var*n_u_dofs, n_x_dofs, n_w_dofs);
      Kzz.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, p_var*n_u_dofs + n_p_dofs+2*n_x_dofs , n_x_dofs, n_z_dofs);



      Fu.reposition (u_var*n_u_dofs, n_u_dofs);
      Fv.reposition (v_var*n_u_dofs, n_v_dofs);
      Fp.reposition (p_var*n_u_dofs, n_p_dofs);
      Fx.reposition (p_var*n_u_dofs + n_p_dofs, n_x_dofs);
      Fy.reposition (p_var*n_u_dofs + n_p_dofs+n_x_dofs, n_y_dofs);
      Fw.reposition (w_var*n_u_dofs, n_w_dofs);
      Fz.reposition (p_var*n_u_dofs + n_p_dofs+2*n_x_dofs, n_y_dofs);
    
	  std::vector<unsigned int> undefo_index;
	  PoroelasticConfig material(dphi,psi);

		
      // Now we will build the element matrix.
      for (unsigned int qp=0; qp<qrule.n_points(); qp++)
        {       
		  
		  
		  Number   p_solid = 0.;

		  grad_u_mat(0) = grad_u_mat(1) = grad_u_mat(2) = 0;
		  for (unsigned int d = 0; d < dim; ++d) {
			std::vector<Number> u_undefo;
			//Fills the vector di with the global degree of freedom indices for the element. :dof_indicies
			aux_system.get_dof_map().dof_indices(elem, undefo_index,d);
			aux_system.current_local_solution->get(undefo_index, u_undefo);
			for (unsigned int l = 0; l != n_u_dofs; l++){
			  grad_u_mat(d).add_scaled(dphi[l][qp], u_undefo[l]); 
			}
		  }
          
		  for (unsigned int l=0; l<n_p_dofs; l++)
		  {
			p_solid += psi[l][qp]*Last_non_linear_soln.current_local_solution->el(dof_indices_p[l]);
		}
		
		Point rX;
				
			for (unsigned int l=0; l<n_u_dofs; l++)
  {
    rX(0) += phi[l][qp]*reference.current_local_solution->el(dof_indices_u[l]);
    rX(1) += phi[l][qp]*reference.current_local_solution->el(dof_indices_v[l]);
    rX(2) += phi[l][qp]*reference.current_local_solution->el(dof_indices_w[l]);
  }

 
	
		material.init_for_qp(rX,grad_u_mat, p_solid, qp,0, p_solid,es);

 
		Real J=material.J;
		Real I_1=material.I_1;
		Real I_2=material.I_2;
		Real I_3=material.I_3;
		

		RealTensor sigma=material.sigma;
		RealTensor sigma_e=material.sigma_e;

		Real sigma_sum_sq=pow(sigma(0,0)*sigma(0,0)+sigma(0,1)*sigma(0,1)+sigma(0,2)*sigma(0,2)+sigma(1,0)*sigma(1,0)+sigma(1,1)*sigma(1,1)+sigma(1,2)*sigma(1,2)+sigma(2,0)*sigma(2,0)+sigma(2,1)*sigma(2,1)+sigma(2,2)*sigma(2,2),0.5);

		sum_jac_postvars=sum_jac_postvars+JxW[qp];
		
		Real av_stress = average_stress(sigma);
  		Real eff_stress = average_stress(sigma_e);

//calculate closest airway resistance to qp point.

Real dist=99999;
Real closest_e=0;

  for (unsigned int i=0; i<tree.number_edges; i++){

		Point dist_e=tree.nodes(tree.edges_lower_node(i));
		
		if(tree.edges_type(i)==0){
			
			Real dist_new=pow(dist_e(0)- q_point_p[0](0),2)+pow(dist_e(1)- q_point_p[0](1),2)+pow(dist_e(2)- q_point_p[0](2),2);
			
			if(dist_new<dist){
				dist=dist_new;
				closest_e=i;
			}
		}	
		
	}

	Real ares=tree.edges_total_resistance(closest_e);

	//std::cout<<"ares "<< ares <<"  q_point "<< q_point[qp] <<" qp "<< qp <<std::endl ;
	
	
  for (unsigned int i=0; i<n_u_dofs; i++){
          Fu(i) += I_1*JxW[qp]*phi[i][qp];
          Fv(i) += I_2*JxW[qp]*phi[i][qp];
          Fw(i) += ares*JxW[qp]*phi[i][qp];

	        Fx(i) += eff_stress*JxW[qp]*phi[i][qp];
          Fy(i) += av_stress*JxW[qp]*phi[i][qp];
          Fz(i) += J*JxW[qp]*phi[i][qp];
	  }
    
    
 
		for (unsigned int i=0; i<n_p_dofs; i++){
            Fp(i) += J*JxW[qp]*psi[i][qp];
		}
    
          
          
     
           //Mass Matrix assembly
          for (unsigned int i=0; i<n_x_dofs; i++){
            for (unsigned int j=0; j<n_x_dofs; j++){
			  Kxx(i,j) += JxW[qp]*(phi[i][qp]*phi[j][qp]);
  			  Kyy(i,j) +=JxW[qp]*(phi[i][qp]*f_phi[j][qp]);
			  Kzz(i,j) += JxW[qp]*(phi[i][qp]*phi[j][qp]);
			  Kuu(i,j) += JxW[qp]*(phi[i][qp]*phi[j][qp]);
  			  Kvv(i,j) +=JxW[qp]*(phi[i][qp]*phi[j][qp]);
			  Kww(i,j) += JxW[qp]*(phi[i][qp]*phi[j][qp]);
			 
			}
		  }
		   for (unsigned int i=0; i<n_p_dofs; i++){
            for (unsigned int j=0; j<n_p_dofs; j++){
              Kpp(i,j) += JxW[qp]*(psi[i][qp]*psi[j][qp]);
			}
		  }
		 

} // end qp





  system.rhs->add_vector(Fe, dof_indices);

  system.matrix->add_matrix (Ke, dof_indices);

} // end of element loop
  
	
    system.matrix->close();
    system.rhs->close();



    std::cout<<"Assemble postvars rhs->l2_norm () "<<system.rhs->l2_norm ()<<std::endl;

		
	 std::cout<<"sum_jac   "<< sum_jac_postvars <<std::endl;
		
  return;
}
