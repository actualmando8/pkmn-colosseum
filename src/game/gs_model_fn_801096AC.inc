    nofralloc
    lfs f0, lbl_8047CE50(r13)
    li r0, 0x1
    lfs f3, lbl_8047AD30(r13)
    fcmpu cr0, f0, f2
    stb r0, lbl_8047AD24(r13)
    stfs f3, lbl_8047AD2C(r13)
    stfs f1, lbl_8047AD34(r13)
    stfs f0, lbl_8047AD30(r13)
    stfs f2, lbl_8047AD38(r13)
    stfs f0, lbl_8047AD3C(r13)
    bnelr
    lfs f0, lbl_8047CE5C(r13)
    stfs f0, lbl_8047AD38(r13)
    stfs f0, lbl_8047AD3C(r13)
    blr

