    nofralloc
    stwu r1, -0x260(r1)
    mflr r0
    stw r0, 0x264(r1)
    stfd f31, 0x250(r1)
    psq_st f31, 0x258(r1), 0, 0
    stfd f30, 0x240(r1)
    psq_st f30, 0x248(r1), 0, 0
    stfd f29, 0x230(r1)
    psq_st f29, 0x238(r1), 0, 0
    stfd f28, 0x220(r1)
    psq_st f28, 0x228(r1), 0, 0
    stfd f27, 0x210(r1)
    psq_st f27, 0x218(r1), 0, 0
    stfd f26, 0x200(r1)
    psq_st f26, 0x208(r1), 0, 0
    stmw r14, 0x1b8(r1)
    mr r14, r3
    cmplwi r14, 0x0
    bne @L_8007D9CC
    li r3, 0xa6
    bl fn_80104704
@L_8007D9CC:
    bl fn_801040A0
    lbz r0, 0x1(r14)
    lwz r15, 0x0(r3)
    extsb r0, r0
    cmpwi r0, 0x3
    mr r18, r15
    beq @L_8007DA64
    bge @L_8007D9F8
    cmpwi r0, 0x0
    beq @L_8007DA04
    b @L_8007DB0C
@L_8007D9F8:
    cmpwi r0, 0x5
    beq @L_8007DA80
    b @L_8007DB0C
@L_8007DA04:
    lbz r0, 0x2(r14)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8007DB0C
    mr r3, r14
    li r4, 0x0
    bl fn_801040D0
    mr r4, r3
    mr r3, r14
    bl fn_8007FDBC
    mr r0, r3
    mr r3, r14
    mr r18, r0
    bl fn_801040A0
    stw r18, 0x0(r3)
    mr r3, r14
    li r4, 0x79a
    li r5, 0x40
    bl fn_801081F8
    mr r3, r14
    li r4, 0x826
    li r5, 0x48
    bl fn_801081F8
    b @L_8007DB0C
@L_8007DA64:
    lbz r0, 0x2(r14)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8007DB0C
    li r0, 0x1
    stb r0, 0x2(r14)
    b @L_8007DB0C
@L_8007DA80:
    lwz r3, 0xb0(r15)
    cmplwi r3, 0x0
    beq @L_8007DACC
    bl fn_800E202C
    mr r14, r3
    rlwinm r0, r14, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8007DAB4
    lis r3, lbl_80268D78@ha
    li r4, 0x1ab
    addi r3, r3, lbl_80268D78@l
    addi r5, r2, -30048
    bl fn_80196E10
@L_8007DAB4:
    mr r3, r14
    bl fn_800E24B0
    mr r3, r14
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0xb0(r15)
@L_8007DACC:
    mr r3, r15
    bl fn_800E202C
    mr r14, r3
    rlwinm r0, r14, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8007DAF8
    lis r3, lbl_80268D78@ha
    li r4, 0x1ab
    addi r3, r3, lbl_80268D78@l
    addi r5, r2, -30048
    bl fn_80196E10
@L_8007DAF8:
    mr r3, r14
    bl fn_800E24B0
    mr r3, r14
    bl fn_800E209C
    b @L_8007FD20
@L_8007DB0C:
    mr r16, r18
    li r19, 0x0
@L_8007DB14:
    lwz r3, 0x118(r16)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x124(r16)
    li r4, 0x0
    bl fn_80109220
    mr r15, r16
    li r17, 0x0
@L_8007DB34:
    lwz r3, 0x130(r15)
    li r4, 0x0
    bl fn_80109220
    addi r15, r15, 0xc
    addi r17, r17, 0x1
    cmpwi r17, 0x2
    blt @L_8007DB34
    addi r16, r16, 0x4
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    blt @L_8007DB14
    mr r15, r18
    li r16, 0x0
@L_8007DB68:
    lwz r3, 0x148(r15)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x150(r15)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x158(r15)
    li r4, 0x0
    bl fn_80109220
    addi r15, r15, 0x4
    addi r16, r16, 0x1
    cmpwi r16, 0x2
    blt @L_8007DB68
    lwz r3, 0x160(r18)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x164(r18)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x168(r18)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x16c(r18)
    li r4, 0x0
    bl fn_80109220
    mr r16, r18
    li r17, 0x0
@L_8007DBD4:
    mr r15, r16
    li r19, 0x0
@L_8007DBDC:
    lwz r3, 0x170(r15)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x290(r15)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x3b0(r15)
    li r4, 0x0
    bl fn_80109220
    addi r15, r15, 0x90
    addi r19, r19, 0x1
    cmpwi r19, 0x2
    blt @L_8007DBDC
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    cmpwi r17, 0x24
    blt @L_8007DBD4
    mr r15, r18
    li r16, 0x0
@L_8007DC28:
    lwz r3, 0x4d8(r15)
    li r4, 0x0
    bl fn_80109220
    addi r15, r15, 0x4
    addi r16, r16, 0x1
    cmplwi r16, 0x2
    blt @L_8007DC28
    mr r15, r18
    li r16, 0x0
@L_8007DC4C:
    lwz r3, 0x4e0(r15)
    li r4, 0x0
    bl fn_80109220
    addi r15, r15, 0x4
    addi r16, r16, 0x1
    cmplwi r16, 0x2
    blt @L_8007DC4C
    lwz r3, 0x4d0(r18)
    li r4, 0x0
    bl fn_80109220
    lwz r3, 0x4d4(r18)
    li r4, 0x0
    bl fn_80109220
    bl fn_801046B8
    lwz r0, 0x4(r14)
    cmpw r0, r3
    bne @L_8007DCF0
    lbz r0, 0xc8(r18)
    cmplwi r0, 0x0
    beq @L_8007DCF0
    lbz r0, 0xc9(r18)
    cmplwi r0, 0x0
    beq @L_8007DCCC
    mr r15, r18
    li r16, 0x0
@L_8007DCB0:
    lwz r3, 0x4d8(r15)
    li r4, 0x1
    bl fn_80109220
    addi r15, r15, 0x4
    addi r16, r16, 0x1
    cmplwi r16, 0x2
    blt @L_8007DCB0
@L_8007DCCC:
    mr r15, r18
    li r16, 0x0
@L_8007DCD4:
    lwz r3, 0x4e0(r15)
    li r4, 0x1
    bl fn_80109220
    addi r15, r15, 0x4
    addi r16, r16, 0x1
    cmplwi r16, 0x2
    blt @L_8007DCD4
@L_8007DCF0:
    bl fn_80105624
    lhz r4, 0x0(r3)
    lhz r3, 0xcc(r18)
    rlwinm r0, r4, 0, 16, 31
    rlwinm r3, r3, 0, 28, 31
    rlwinm r0, r0, 0, 28, 31
    cmpw r3, r0
    beq @L_8007DD18
    li r0, 0x0
    stb r0, 0xca(r18)
@L_8007DD18:
    sth r4, 0xcc(r18)
    lwz r0, 0xac(r18)
    cmpwi r0, 0x0
    ble @L_8007DD34
    lwz r0, 0xa4(r18)
    cmpwi r0, 0x0
    bge @L_8007DD3C
@L_8007DD34:
    li r0, 0x0
    b @L_8007DD48
@L_8007DD3C:
    lwz r3, 0xb0(r18)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007DD48:
    li r4, 0x0
    cmplwi r0, 0x0
    stw r0, 0xa8(r1)
    stw r4, 0xac(r1)
    bne @L_8007DDB8
    lwz r3, 0x4d4(r18)
    li r4, 0x1
    bl fn_80109220
    bl fn_801046B8
    lwz r0, 0x4(r14)
    cmpw r0, r3
    bne @L_8007FD20
    lbz r0, 0xc8(r18)
    cmplwi r0, 0x0
    beq @L_8007FD20
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 28, 31
    cmpwi r0, 0x0
    beq @L_8007FD20
    lbz r0, 0xca(r18)
    cmplwi r0, 0x0
    bne @L_8007DDAC
    li r3, 0x26
    bl fn_80166A28
@L_8007DDAC:
    li r0, 0x1
    stb r0, 0xca(r18)
    b @L_8007FD20
@L_8007DDB8:
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    beq @L_8007DE14
    lwz r3, 0xb8(r18)
    addi r0, r3, 0x1
    stw r0, 0xb8(r18)
    lwz r0, 0xb8(r18)
    cmpwi r0, 0x10
    bne @L_8007DDEC
    stw r4, 0xb8(r18)
    lfs f26, -30040(r2)
    stb r4, 0xb6(r18)
    b @L_8007E0BC
@L_8007DDEC:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0xb4(r1)
    lfd f2, -30008(r2)
    stw r0, 0xb0(r1)
    lfs f0, -30036(r2)
    lfd f1, 0xb0(r1)
    fsubs f1, f1, f2
    fmuls f26, f1, f0
    b @L_8007E0BC
@L_8007DE14:
    lwz r0, 0xa4(r18)
    stw r0, 0xa0(r18)
    lbz r0, 0xb5(r18)
    stb r0, 0xb4(r18)
    bl fn_801046B8
    lwz r0, 0x4(r14)
    cmpw r0, r3
    bne @L_8007E018
    lbz r0, 0xc8(r18)
    cmplwi r0, 0x0
    beq @L_8007E018
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_8007DEA0
    lwz r0, 0xa4(r18)
    cmpwi r0, 0x0
    bne @L_8007DE80
    lbz r0, 0xca(r18)
    cmplwi r0, 0x0
    bne @L_8007DE74
    li r3, 0x26
    bl fn_80166A28
@L_8007DE74:
    li r0, 0x1
    stb r0, 0xca(r18)
    b @L_8007E018
@L_8007DE80:
    li r3, 0x3c5
    bl fn_80166A28
    lwz r3, 0xa4(r18)
    li r0, 0x1
    subi r3, r3, 0x1
    stw r3, 0xa4(r18)
    stb r0, 0xb6(r18)
    b @L_8007E018
@L_8007DEA0:
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8007DF08
    lwz r3, 0xac(r18)
    lwz r4, 0xa4(r18)
    subi r0, r3, 0x1
    cmpw r4, r0
    bne @L_8007DEE8
    lbz r0, 0xca(r18)
    cmplwi r0, 0x0
    bne @L_8007DEDC
    li r3, 0x26
    bl fn_80166A28
@L_8007DEDC:
    li r0, 0x1
    stb r0, 0xca(r18)
    b @L_8007E018
@L_8007DEE8:
    li r3, 0x3c5
    bl fn_80166A28
    lwz r3, 0xa4(r18)
    li r0, 0x1
    addi r3, r3, 0x1
    stw r3, 0xa4(r18)
    stb r0, 0xb6(r18)
    b @L_8007E018
@L_8007DF08:
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8007DF6C
    lbz r0, 0xb5(r18)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8007DF4C
    lbz r0, 0xca(r18)
    cmplwi r0, 0x0
    bne @L_8007DF40
    li r3, 0x26
    bl fn_80166A28
@L_8007DF40:
    li r0, 0x1
    stb r0, 0xca(r18)
    b @L_8007E018
@L_8007DF4C:
    li r3, 0x3c5
    bl fn_80166A28
    lbz r3, 0xb5(r18)
    li r0, 0x1
    subi r3, r3, 0x1
    stb r3, 0xb5(r18)
    stb r0, 0xb6(r18)
    b @L_8007E018
@L_8007DF6C:
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8007E018
    lwz r0, 0xa8(r1)
    cmplwi r0, 0x0
    bne @L_8007DFA4
    lis r3, lbl_80268D78@ha
    lis r5, lbl_80268DB4@ha
    addi r3, r3, lbl_80268D78@l
    li r4, 0x28e
    addi r5, r5, lbl_80268DB4@l
    bl fn_80196E10
@L_8007DFA4:
    lwz r3, 0xa8(r1)
    lbz r5, 0xb5(r18)
    lbz r0, 0x1b(r3)
    extsb r5, r5
    extsb r4, r0
    subi r0, r4, 0x1
    cmpw r5, r0
    beq @L_8007DFDC
    addi r0, r5, 0x1
    extsb r4, r0
    bl fn_80082A88
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8007DFFC
@L_8007DFDC:
    lbz r0, 0xca(r18)
    cmplwi r0, 0x0
    bne @L_8007DFF0
    li r3, 0x26
    bl fn_80166A28
@L_8007DFF0:
    li r0, 0x1
    stb r0, 0xca(r18)
    b @L_8007E018
@L_8007DFFC:
    li r3, 0x3c5
    bl fn_80166A28
    lbz r3, 0xb5(r18)
    li r0, 0x1
    addi r3, r3, 0x1
    stb r3, 0xb5(r18)
    stb r0, 0xb6(r18)
@L_8007E018:
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    beq @L_8007E0B8
    lwz r0, 0xac(r18)
    cmpwi r0, 0x0
    ble @L_8007E03C
    lwz r0, 0xa4(r18)
    cmpwi r0, 0x0
    bge @L_8007E044
@L_8007E03C:
    li r3, 0x0
    b @L_8007E050
@L_8007E044:
    lwz r3, 0xb0(r18)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r3, r0
@L_8007E050:
    li r0, 0x0
    stw r3, 0xa8(r1)
    stw r0, 0xbc(r18)
    stw r0, 0xb8(r18)
    lbz r4, 0x1b(r3)
    lbz r0, 0xb5(r18)
    extsb r3, r4
    extsb r0, r0
    cmpw r3, r0
    bgt @L_8007E084
    subi r0, r4, 0x1
    extsb r0, r0
    stb r0, 0xb5(r18)
@L_8007E084:
    lwz r14, 0xa8(r1)
    b @L_8007E098
@L_8007E08C:
    lbz r3, 0xb5(r18)
    subi r0, r3, 0x1
    stb r0, 0xb5(r18)
@L_8007E098:
    lbz r4, 0xb5(r18)
    mr r3, r14
    bl fn_80082A88
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8007E08C
    lfs f26, -30032(r2)
    b @L_8007E0BC
@L_8007E0B8:
    lfs f26, -30040(r2)
@L_8007E0BC:
    addi r0, r1, 0xa4
    stw r0, 0x1ac(r1)
    lwz r0, 0xac(r18)
    cmpwi r0, 0x0
    ble @L_8007E0DC
    lwz r0, 0xa0(r18)
    cmpwi r0, 0x0
    bge @L_8007E0E4
@L_8007E0DC:
    li r0, 0x0
    b @L_8007E0F0
@L_8007E0E4:
    lwz r3, 0xb0(r18)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007E0F0:
    stw r0, 0xa4(r1)
    lwz r0, 0xac(r18)
    cmpwi r0, 0x0
    ble @L_8007E10C
    lwz r0, 0xa4(r18)
    cmpwi r0, 0x0
    bge @L_8007E114
@L_8007E10C:
    li r0, 0x0
    b @L_8007E120
@L_8007E114:
    lwz r3, 0xb0(r18)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007E120:
    stw r0, 0xa8(r1)
    lfs f0, -30040(r2)
    li r0, 0x0
    lfs f1, -30028(r2)
    mr r22, r18
    fsubs f27, f0, f26
    lfs f0, -30024(r2)
    fmuls f30, f1, f26
    lwz r23, 0x1ac(r1)
    fmuls f31, f0, f26
    stw r0, 0x1a8(r1)
    fmuls f29, f1, f27
    fmuls f28, f0, f27
@L_8007E154:
    lwz r6, 0x0(r23)
    li r0, -0x1
    lha r4, 0xd2(r18)
    li r3, -0x1
    cmplwi r6, 0x0
    lha r15, 0xd4(r18)
    sth r4, 0x1a4(r1)
    beq @L_8007E700
    lbz r4, 0xb6(r18)
    cmplwi r4, 0x0
    bne @L_8007E18C
    lwz r4, 0x1a8(r1)
    cmpwi r4, 0x0
    beq @L_8007E700
@L_8007E18C:
    lbz r5, 0x1d(r6)
    lwz r4, 0x1a8(r1)
    extsb r5, r5
    lha r8, 0xce(r18)
    subfic r5, r5, 0x5
    cmpwi r4, 0x1
    mullw r7, r15, r5
    lbz r4, 0x1c(r6)
    lha r6, 0xd0(r18)
    extsb r4, r4
    rlwinm r5, r7, 1, 31, 31
    add r5, r5, r7
    srawi r7, r5, 1
    subfic r5, r4, 0x6
    lha r4, 0x1a4(r1)
    mullw r5, r4, r5
    add r4, r8, r7
    extsh r16, r4
    rlwinm r4, r5, 1, 31, 31
    add r4, r4, r5
    srawi r4, r4, 1
    add r4, r6, r4
    extsh r17, r4
    beq @L_8007E2F8
    bge @L_8007E3EC
    lwz r4, 0x1a8(r1)
    cmpwi r4, 0x0
    bge @L_8007E200
    b @L_8007E3EC
@L_8007E200:
    lbz r3, 0xb4(r18)
    lbz r5, 0xb5(r18)
    extsb r4, r3
    extsb r3, r5
    cmpw r4, r3
    bge @L_8007E248
    xoris r4, r16, 0x8000
    lis r3, 0x4330
    stw r4, 0xb4(r1)
    lfd f1, -30008(r2)
    stw r3, 0xb0(r1)
    lfd f0, 0xb0(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f30
    fctiwz f0, f0
    stfd f0, 0xb8(r1)
    lwz r16, 0xbc(r1)
    b @L_8007E27C
@L_8007E248:
    cmpw r3, r4
    bge @L_8007E27C
    xoris r4, r16, 0x8000
    lis r3, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r3, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fadds f0, f0, f30
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r16, 0xb4(r1)
@L_8007E27C:
    lwz r3, 0xa0(r18)
    lwz r4, 0xa4(r18)
    cmpw r3, r4
    bge @L_8007E2BC
    xoris r4, r17, 0x8000
    lis r3, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r3, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r17, 0xb4(r1)
    b @L_8007E2F0
@L_8007E2BC:
    cmpw r4, r3
    bge @L_8007E2F0
    xoris r4, r17, 0x8000
    lis r3, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r3, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fadds f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r17, 0xb4(r1)
@L_8007E2F0:
    li r3, -0x100
    b @L_8007E3EC
@L_8007E2F8:
    lbz r0, 0xb4(r18)
    lbz r5, 0xb5(r18)
    extsb r4, r0
    extsb r0, r5
    cmpw r4, r0
    bge @L_8007E340
    xoris r4, r16, 0x8000
    lis r0, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r0, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fadds f0, f0, f29
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r16, 0xb4(r1)
    b @L_8007E374
@L_8007E340:
    cmpw r0, r4
    bge @L_8007E374
    xoris r4, r16, 0x8000
    lis r0, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r0, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f29
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r16, 0xb4(r1)
@L_8007E374:
    lwz r0, 0xa0(r18)
    lwz r4, 0xa4(r18)
    cmpw r0, r4
    bge @L_8007E3B4
    xoris r4, r17, 0x8000
    lis r0, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r0, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fadds f0, f0, f28
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r17, 0xb4(r1)
    b @L_8007E3E8
@L_8007E3B4:
    cmpw r4, r0
    bge @L_8007E3E8
    xoris r4, r17, 0x8000
    lis r0, 0x4330
    stw r4, 0xbc(r1)
    lfd f1, -30008(r2)
    stw r0, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f28
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r17, 0xb4(r1)
@L_8007E3E8:
    li r0, -0x100
@L_8007E3EC:
    stw r0, 0x98(r1)
    lis r7, 0x4330
    lwz r4, 0x1a8(r1)
    stw r3, 0x9c(r1)
    lbz r8, 0x98(r1)
    addi r21, r4, 0xb4
    lbz r3, 0x9c(r1)
    lbz r9, 0x99(r1)
    subf r3, r8, r3
    stw r7, 0xb8(r1)
    xoris r4, r3, 0x8000
    lbz r0, 0x9d(r1)
    stw r4, 0xbc(r1)
    subf r0, r9, r0
    lfd f4, -30008(r2)
    xoris r6, r0, 0x8000
    lfd f0, 0xb8(r1)
    stw r8, 0xb4(r1)
    lfd f3, -30000(r2)
    fsubs f2, f0, f4
    stw r7, 0xb0(r1)
    lbz r10, 0x9a(r1)
    lfd f0, 0xb0(r1)
    lbz r3, 0x9e(r1)
    fsubs f1, f0, f3
    stw r6, 0xcc(r1)
    subf r3, r10, r3
    lbz r11, 0x9b(r1)
    xoris r5, r3, 0x8000
    stw r7, 0xc8(r1)
    fmadds f2, f26, f2, f1
    lfd f0, 0xc8(r1)
    stw r9, 0xd4(r1)
    lbz r0, 0x9f(r1)
    fsubs f1, f0, f4
    stw r7, 0xd0(r1)
    fctiwz f2, f2
    subf r0, r11, r0
    lfd f0, 0xd0(r1)
    xoris r0, r0, 0x8000
    stw r5, 0xe4(r1)
    fsubs f0, f0, f3
    stfd f2, 0xc0(r1)
    lwz r3, 0x0(r23)
    fmadds f1, f26, f1, f0
    stw r7, 0xe0(r1)
    lwz r6, 0xc4(r1)
    lfd f0, 0xe0(r1)
    fctiwz f2, f1
    stw r10, 0xec(r1)
    fsubs f1, f0, f4
    lbzx r4, r18, r21
    stw r7, 0xe8(r1)
    lfd f0, 0xe8(r1)
    stfd f2, 0xd8(r1)
    fsubs f0, f0, f3
    stw r0, 0xfc(r1)
    lwz r5, 0xdc(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0xf8(r1)
    lfd f0, 0xf8(r1)
    fctiwz f2, f1
    stw r11, 0x104(r1)
    fsubs f1, f0, f4
    stw r7, 0x100(r1)
    lfd f0, 0x100(r1)
    stfd f2, 0xf0(r1)
    fsubs f0, f0, f3
    lwz r0, 0xf4(r1)
    stb r6, 0xa0(r1)
    fmadds f0, f26, f1, f0
    stb r0, 0xa2(r1)
    fctiwz f0, f0
    stb r5, 0xa1(r1)
    stfd f0, 0x108(r1)
    lwz r0, 0x10c(r1)
    stb r0, 0xa3(r1)
    lwz r29, 0xa0(r1)
    bl fn_80082FE4
    extsh r0, r17
    li r14, 0x0
    stw r14, 0x1b4(r1)
    extsh r24, r16
    mr r25, r3
    li r17, 0x0
    stw r0, 0x1b0(r1)
    b @L_8007E6E8
@L_8007E548:
    lwz r3, 0x1b0(r1)
    mr r28, r14
    lwz r0, 0x1b4(r1)
    li r16, 0x0
    li r27, 0x0
    add r26, r3, r0
    extsh r30, r26
    rlwinm r31, r14, 2, 0, 29
    b @L_8007E6C0
@L_8007E56C:
    add r20, r22, r31
    add r19, r24, r27
    lwz r5, 0x170(r20)
    extsh r0, r19
    lwz r3, 0x0(r23)
    extsb r4, r16
    sth r0, 0x50(r5)
    lwz r5, 0x170(r20)
    sth r30, 0x52(r5)
    lbz r5, 0x1d(r3)
    extsb r0, r5
    cmpw r4, r0
    bne @L_8007E5AC
    lbz r0, 0x1c(r25)
    cmplwi r0, 0x0
    bne @L_8007E5D8
@L_8007E5AC:
    extsb r4, r16
    extsb r0, r5
    cmpw r4, r0
    bge @L_8007E6B0
    lbzx r4, r18, r21
    mr r5, r17
    mr r6, r16
    bl fn_80082EA4
    lbz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_8007E6B0
@L_8007E5D8:
    lwz r0, 0xbc(r18)
    cmpwi r0, 0x0
    beq @L_8007E604
    cmpwi r0, 0x6
    beq @L_8007E604
    lwz r0, 0xc0(r18)
    cmpw r0, r28
    bne @L_8007E604
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    bne @L_8007E6B0
@L_8007E604:
    lwz r3, 0x170(r20)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x3b0(r20)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x290(r20)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x170(r20)
    stw r29, 0x64(r3)
    lha r0, 0xd6(r18)
    lha r4, 0xce(r18)
    add r0, r0, r19
    lwz r3, 0x3b0(r20)
    subf r0, r4, r0
    extsh r0, r0
    sth r0, 0x50(r3)
    lha r0, 0xd8(r18)
    lha r4, 0xd0(r18)
    add r0, r0, r26
    lwz r3, 0x3b0(r20)
    subf r0, r4, r0
    extsh r0, r0
    sth r0, 0x52(r3)
    lwz r3, 0x3b0(r20)
    stw r29, 0x64(r3)
    lha r0, 0xde(r18)
    lha r4, 0xce(r18)
    add r0, r0, r19
    lwz r3, 0x290(r20)
    subf r0, r4, r0
    extsh r0, r0
    sth r0, 0x50(r3)
    lha r0, 0xe0(r18)
    lha r4, 0xd0(r18)
    add r0, r0, r26
    lwz r3, 0x290(r20)
    subf r0, r4, r0
    extsh r0, r0
    sth r0, 0x52(r3)
    lwz r3, 0x290(r20)
    stw r29, 0x64(r3)
@L_8007E6B0:
    addi r28, r28, 0x1
    addi r31, r31, 0x4
    add r27, r27, r15
    addi r16, r16, 0x1
@L_8007E6C0:
    extsb r0, r16
    cmpwi r0, 0x6
    blt @L_8007E56C
    lwz r3, 0x1b4(r1)
    addi r14, r14, 0x6
    lha r0, 0x1a4(r1)
    addi r25, r25, 0xe
    addi r17, r17, 0x1
    add r3, r3, r0
    stw r3, 0x1b4(r1)
@L_8007E6E8:
    lwz r3, 0x0(r23)
    extsb r4, r17
    lbz r0, 0x1c(r3)
    extsb r0, r0
    cmpw r4, r0
    blt @L_8007E548
@L_8007E700:
    lwz r3, 0x1a8(r1)
    addi r23, r23, 0x4
    addi r22, r22, 0x90
    addi r3, r3, 0x1
    stw r3, 0x1a8(r1)
    mr r0, r3
    cmpwi r0, 0x2
    blt @L_8007E154
    lwz r17, 0x1ac(r1)
    mr r19, r18
    li r20, 0x0
@L_8007E72C:
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    bne @L_8007E740
    cmpwi r20, 0x0
    beq @L_8007E904
@L_8007E740:
    lwz r0, 0x0(r17)
    cmplwi r0, 0x0
    beq @L_8007E904
    mr r15, r19
    addi r16, r20, 0xb4
    li r21, 0x0
    lfd f31, -30008(r2)
    lis r14, 0x4330
    lfd f30, -30000(r2)
    b @L_8007E8F0
@L_8007E768:
    lbzx r0, r18, r16
    extsb r0, r0
    cmpw r21, r0
    bne @L_8007E780
    li r22, -0x1
    b @L_8007E788
@L_8007E780:
    lis r3, 0x250
    subi r22, r3, 0x7b01
@L_8007E788:
    cmpwi r20, 0x1
    beq @L_8007E7AC
    bge @L_8007E7B4
    cmpwi r20, 0x0
    bge @L_8007E7A0
    b @L_8007E7B4
@L_8007E7A0:
    rlwinm r0, r22, 0, 0, 23
    stw r0, 0x1a0(r1)
    b @L_8007E7B4
@L_8007E7AC:
    stw r22, 0x1a0(r1)
    rlwinm r22, r22, 0, 0, 23
@L_8007E7B4:
    lwz r3, 0x130(r15)
    li r4, 0x1
    bl fn_80109220
    lwz r0, 0x1a0(r1)
    addi r21, r21, 0x1
    stw r22, 0x8c(r1)
    lwz r3, 0x130(r15)
    addi r15, r15, 0x4
    stw r0, 0x90(r1)
    lbz r7, 0x8c(r1)
    lbz r4, 0x90(r1)
    lbz r8, 0x8d(r1)
    subf r4, r7, r4
    lbz r0, 0x91(r1)
    xoris r6, r4, 0x8000
    stw r14, 0x108(r1)
    subf r0, r8, r0
    lbz r9, 0x8e(r1)
    stw r6, 0x10c(r1)
    xoris r5, r0, 0x8000
    lbz r4, 0x92(r1)
    lfd f0, 0x108(r1)
    stw r7, 0x104(r1)
    subf r4, r9, r4
    fsubs f2, f0, f31
    xoris r4, r4, 0x8000
    stw r14, 0x100(r1)
    lbz r10, 0x8f(r1)
    lfd f0, 0x100(r1)
    stw r5, 0xf4(r1)
    fsubs f1, f0, f30
    lbz r0, 0x93(r1)
    stw r14, 0xf0(r1)
    subf r0, r10, r0
    fmadds f2, f26, f2, f1
    lfd f0, 0xf0(r1)
    stw r8, 0xec(r1)
    xoris r0, r0, 0x8000
    fsubs f1, f0, f31
    stw r14, 0xe8(r1)
    fctiwz f2, f2
    lfd f0, 0xe8(r1)
    stw r4, 0xdc(r1)
    fsubs f0, f0, f30
    stfd f2, 0xf8(r1)
    fmadds f1, f26, f1, f0
    stw r14, 0xd8(r1)
    lwz r5, 0xfc(r1)
    lfd f0, 0xd8(r1)
    fctiwz f2, f1
    stw r9, 0xd4(r1)
    fsubs f1, f0, f31
    stw r14, 0xd0(r1)
    lfd f0, 0xd0(r1)
    stfd f2, 0xe0(r1)
    fsubs f0, f0, f30
    stw r0, 0xc4(r1)
    lwz r4, 0xe4(r1)
    fmadds f1, f26, f1, f0
    stw r14, 0xc0(r1)
    lfd f0, 0xc0(r1)
    fctiwz f2, f1
    stw r10, 0xbc(r1)
    fsubs f1, f0, f31
    stw r14, 0xb8(r1)
    lfd f0, 0xb8(r1)
    stfd f2, 0xc8(r1)
    fsubs f0, f0, f30
    lwz r0, 0xcc(r1)
    stb r5, 0x94(r1)
    fmadds f0, f26, f1, f0
    stb r0, 0x96(r1)
    fctiwz f0, f0
    stb r4, 0x95(r1)
    stfd f0, 0xb0(r1)
    lwz r0, 0xb4(r1)
    stb r0, 0x97(r1)
    lwz r0, 0x94(r1)
    stw r0, 0x64(r3)
@L_8007E8F0:
    lwz r3, 0x0(r17)
    lbz r0, 0x1b(r3)
    extsb r0, r0
    cmpw r21, r0
    blt @L_8007E768
@L_8007E904:
    addi r17, r17, 0x4
    addi r19, r19, 0xc
    addi r20, r20, 0x1
    cmpwi r20, 0x2
    blt @L_8007E72C
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    beq @L_8007EAFC
    lwz r4, 0xa4(r1)
    lwz r3, 0xa8(r1)
    cmplwi r4, 0x0
    lbz r5, 0x1b(r3)
    beq @L_8007E950
    lbz r4, 0x1b(r4)
    extsb r3, r5
    extsb r0, r4
    cmpw r3, r0
    bge @L_8007E950
    mr r5, r4
@L_8007E950:
    mr r16, r18
    li r19, 0x0
    lwz r17, 0xa4(r1)
    extsb r20, r5
    lwz r15, 0xa8(r1)
    lis r14, 0x4330
    lfd f30, -30008(r2)
    lfd f31, -30000(r2)
    b @L_8007EAEC
@L_8007E974:
    lwz r3, 0x124(r16)
    li r21, -0x1
    li r22, -0x1
    li r4, 0x1
    bl fn_80109220
    cmplwi r17, 0x0
    beq @L_8007E9A4
    lbz r3, 0x1b(r17)
    extsb r0, r19
    extsb r3, r3
    cmpw r3, r0
    bgt @L_8007E9A8
@L_8007E9A4:
    li r21, -0x100
@L_8007E9A8:
    lbz r3, 0x1b(r15)
    extsb r0, r19
    extsb r3, r3
    cmpw r3, r0
    bgt @L_8007E9C0
    li r22, -0x100
@L_8007E9C0:
    stw r21, 0x80(r1)
    addi r19, r19, 0x1
    lwz r3, 0x124(r16)
    addi r16, r16, 0x4
    stw r22, 0x84(r1)
    lbz r7, 0x80(r1)
    lbz r4, 0x84(r1)
    lbz r8, 0x81(r1)
    lbz r0, 0x85(r1)
    subf r4, r7, r4
    lbz r9, 0x82(r1)
    xoris r6, r4, 0x8000
    lbz r4, 0x86(r1)
    subf r0, r8, r0
    lbz r10, 0x83(r1)
    xoris r5, r0, 0x8000
    lbz r0, 0x87(r1)
    subf r4, r9, r4
    xoris r4, r4, 0x8000
    stw r6, 0x10c(r1)
    subf r0, r10, r0
    xoris r0, r0, 0x8000
    stw r14, 0x108(r1)
    lfd f0, 0x108(r1)
    stw r7, 0x104(r1)
    fsubs f2, f0, f30
    stw r14, 0x100(r1)
    lfd f0, 0x100(r1)
    stw r5, 0xf4(r1)
    fsubs f1, f0, f31
    stw r14, 0xf0(r1)
    lfd f0, 0xf0(r1)
    fmadds f2, f26, f2, f1
    stw r8, 0xec(r1)
    fsubs f1, f0, f30
    stw r14, 0xe8(r1)
    fctiwz f2, f2
    lfd f0, 0xe8(r1)
    stfd f2, 0xf8(r1)
    fsubs f0, f0, f31
    lwz r5, 0xfc(r1)
    stw r4, 0xdc(r1)
    fmadds f1, f26, f1, f0
    stw r14, 0xd8(r1)
    lfd f0, 0xd8(r1)
    fctiwz f2, f1
    stw r9, 0xd4(r1)
    fsubs f1, f0, f30
    stw r14, 0xd0(r1)
    lfd f0, 0xd0(r1)
    stfd f2, 0xe0(r1)
    fsubs f0, f0, f31
    lwz r4, 0xe4(r1)
    stw r0, 0xc4(r1)
    fmadds f1, f26, f1, f0
    stw r14, 0xc0(r1)
    lfd f0, 0xc0(r1)
    fctiwz f2, f1
    stw r10, 0xbc(r1)
    fsubs f1, f0, f30
    stw r14, 0xb8(r1)
    lfd f0, 0xb8(r1)
    stfd f2, 0xc8(r1)
    fsubs f0, f0, f31
    lwz r0, 0xcc(r1)
    stb r5, 0x88(r1)
    fmadds f0, f26, f1, f0
    stb r4, 0x89(r1)
    fctiwz f0, f0
    stb r0, 0x8a(r1)
    stfd f0, 0xb0(r1)
    lwz r0, 0xb4(r1)
    stb r0, 0x8b(r1)
    lwz r0, 0x88(r1)
    stw r0, 0x64(r3)
@L_8007EAEC:
    extsb r0, r19
    cmpw r0, r20
    blt @L_8007E974
    b @L_8007EB40
@L_8007EAFC:
    mr r16, r18
    li r17, 0x0
    lwz r14, 0xa8(r1)
    li r15, -0x1
    b @L_8007EB2C
@L_8007EB10:
    lwz r3, 0x124(r16)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x124(r16)
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    stw r15, 0x64(r3)
@L_8007EB2C:
    lbz r0, 0x1b(r14)
    extsb r3, r17
    extsb r0, r0
    cmpw r3, r0
    blt @L_8007EB10
@L_8007EB40:
    lwz r3, 0x118(r18)
    li r4, 0x1
    bl fn_80109220
    lwz r0, 0xa4(r1)
    cmplwi r0, 0x0
    beq @L_8007EBD0
    lbz r4, 0xb4(r18)
    lis r0, 0x4330
    lbz r3, 0xb5(r18)
    extsb r4, r4
    stw r0, 0x108(r1)
    extsb r3, r3
    lfd f2, -30008(r2)
    rlwinm r4, r4, 3, 0, 28
    stw r0, 0x100(r1)
    rlwinm r0, r3, 3, 0, 28
    lwz r3, 0x118(r18)
    add r5, r18, r4
    add r4, r18, r0
    lha r5, 0xe6(r5)
    lha r4, 0xe6(r4)
    xoris r0, r5, 0x8000
    subf r4, r5, r4
    stw r0, 0x104(r1)
    xoris r0, r4, 0x8000
    stw r0, 0x10c(r1)
    lfd f0, 0x100(r1)
    lfd f1, 0x108(r1)
    fsubs f0, f0, f2
    fsubs f1, f1, f2
    fmadds f0, f26, f1, f0
    fctiwz f0, f0
    stfd f0, 0xf8(r1)
    lwz r0, 0xfc(r1)
    sth r0, 0x50(r3)
    b @L_8007EBEC
@L_8007EBD0:
    lbz r0, 0xb5(r18)
    lwz r3, 0x118(r18)
    extsb r0, r0
    rlwinm r0, r0, 3, 0, 28
    add r4, r18, r0
    lha r0, 0xe6(r4)
    sth r0, 0x50(r3)
@L_8007EBEC:
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    beq @L_8007EF40
    lwz r0, 0xa4(r18)
    lwz r3, 0xa0(r18)
    cmpw r0, r3
    beq @L_8007EF40
    cmpw r3, r0
    bge @L_8007EC18
    li r14, 0x1
    b @L_8007EC1C
@L_8007EC18:
    li r14, -0x1
@L_8007EC1C:
    lwz r3, 0x158(r18)
    li r4, 0x1
    bl fn_80109220
    li r0, -0x1
    li r3, -0x100
    stw r0, 0x74(r1)
    lis r7, 0x4330
    xoris r0, r14, 0x8000
    lfs f0, -30020(r2)
    stw r3, 0x78(r1)
    lbz r9, 0x74(r1)
    fmuls f2, f0, f26
    lbz r4, 0x78(r1)
    lbz r10, 0x75(r1)
    lbz r3, 0x79(r1)
    subf r4, r9, r4
    lbz r11, 0x76(r1)
    xoris r8, r4, 0x8000
    lbz r4, 0x7a(r1)
    subf r3, r10, r3
    lbz r12, 0x77(r1)
    xoris r6, r3, 0x8000
    lbz r3, 0x7b(r1)
    subf r4, r11, r4
    xoris r5, r4, 0x8000
    stw r8, 0x10c(r1)
    subf r3, r12, r3
    lfd f5, -30008(r2)
    xoris r4, r3, 0x8000
    stw r7, 0x108(r1)
    lfd f4, -30000(r2)
    lfd f0, 0x108(r1)
    stw r9, 0x104(r1)
    fsubs f3, f0, f5
    lwz r3, 0x158(r18)
    stw r7, 0x100(r1)
    lfd f0, 0x100(r1)
    stw r6, 0xf4(r1)
    fsubs f1, f0, f4
    stw r7, 0xf0(r1)
    lfd f0, 0xf0(r1)
    fmadds f3, f26, f3, f1
    stw r10, 0xec(r1)
    fsubs f1, f0, f5
    stw r7, 0xe8(r1)
    fctiwz f3, f3
    lfd f0, 0xe8(r1)
    stfd f3, 0xf8(r1)
    fsubs f0, f0, f4
    lwz r6, 0xfc(r1)
    stw r5, 0xdc(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0xd8(r1)
    lfd f0, 0xd8(r1)
    fctiwz f3, f1
    stw r11, 0xd4(r1)
    fsubs f1, f0, f5
    stw r7, 0xd0(r1)
    lfd f0, 0xd0(r1)
    stfd f3, 0xe0(r1)
    fsubs f0, f0, f4
    lwz r5, 0xe4(r1)
    stw r4, 0xc4(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0xc0(r1)
    lfd f0, 0xc0(r1)
    fctiwz f3, f1
    stw r12, 0xbc(r1)
    fsubs f1, f0, f5
    stw r7, 0xb8(r1)
    lfd f0, 0xb8(r1)
    stfd f3, 0xc8(r1)
    fsubs f0, f0, f4
    lwz r4, 0xcc(r1)
    stb r6, 0x7c(r1)
    fmadds f0, f26, f1, f0
    stb r5, 0x7d(r1)
    fctiwz f0, f0
    stb r4, 0x7e(r1)
    stw r0, 0x114(r1)
    stfd f0, 0xb0(r1)
    lwz r0, 0xb4(r1)
    stw r7, 0x110(r1)
    stb r0, 0x7f(r1)
    lfd f0, 0x110(r1)
    lwz r0, 0x7c(r1)
    stw r7, 0x118(r1)
    fsubs f1, f0, f5
    stw r0, 0x64(r3)
    lha r0, 0x100(r18)
    lwz r3, 0x158(r18)
    xoris r0, r0, 0x8000
    stw r0, 0x11c(r1)
    lfd f0, 0x118(r1)
    fsubs f0, f0, f5
    fnmsubs f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x120(r1)
    lwz r0, 0x124(r1)
    sth r0, 0x52(r3)
    lwz r3, 0x15c(r18)
    li r4, 0x1
    bl fn_80109220
    li r0, -0x100
    li r3, -0x1
    stw r0, 0x68(r1)
    lis r7, 0x4330
    xoris r0, r14, 0x8000
    lfs f0, -30020(r2)
    stw r3, 0x6c(r1)
    lbz r9, 0x68(r1)
    fmuls f2, f0, f27
    lbz r4, 0x6c(r1)
    lbz r10, 0x69(r1)
    lbz r3, 0x6d(r1)
    subf r4, r9, r4
    lbz r11, 0x6a(r1)
    xoris r8, r4, 0x8000
    lbz r4, 0x6e(r1)
    subf r3, r10, r3
    lbz r12, 0x6b(r1)
    xoris r6, r3, 0x8000
    lbz r3, 0x6f(r1)
    subf r4, r11, r4
    xoris r5, r4, 0x8000
    stw r8, 0x12c(r1)
    subf r3, r12, r3
    lfd f5, -30008(r2)
    xoris r4, r3, 0x8000
    stw r7, 0x128(r1)
    lfd f4, -30000(r2)
    lfd f0, 0x128(r1)
    stw r9, 0x134(r1)
    fsubs f3, f0, f5
    lwz r3, 0x15c(r18)
    stw r7, 0x130(r1)
    lfd f0, 0x130(r1)
    stw r6, 0x144(r1)
    fsubs f1, f0, f4
    stw r7, 0x140(r1)
    lfd f0, 0x140(r1)
    fmadds f3, f26, f3, f1
    stw r10, 0x14c(r1)
    fsubs f1, f0, f5
    stw r7, 0x148(r1)
    fctiwz f3, f3
    lfd f0, 0x148(r1)
    stfd f3, 0x138(r1)
    fsubs f0, f0, f4
    lwz r6, 0x13c(r1)
    stw r5, 0x15c(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0x158(r1)
    lfd f0, 0x158(r1)
    fctiwz f3, f1
    stw r11, 0x164(r1)
    fsubs f1, f0, f5
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    stfd f3, 0x150(r1)
    fsubs f0, f0, f4
    lwz r5, 0x154(r1)
    stw r4, 0x174(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0x170(r1)
    lfd f0, 0x170(r1)
    fctiwz f3, f1
    stw r12, 0x17c(r1)
    fsubs f1, f0, f5
    stw r7, 0x178(r1)
    lfd f0, 0x178(r1)
    stfd f3, 0x168(r1)
    fsubs f0, f0, f4
    lwz r4, 0x16c(r1)
    stb r6, 0x70(r1)
    fmadds f0, f26, f1, f0
    stb r5, 0x71(r1)
    fctiwz f0, f0
    stb r4, 0x72(r1)
    stw r0, 0x18c(r1)
    stfd f0, 0x180(r1)
    lwz r0, 0x184(r1)
    stw r7, 0x188(r1)
    stb r0, 0x73(r1)
    lfd f0, 0x188(r1)
    lwz r0, 0x70(r1)
    stw r7, 0x190(r1)
    fsubs f1, f0, f5
    stw r0, 0x64(r3)
    lha r0, 0x100(r18)
    lwz r3, 0x15c(r18)
    xoris r0, r0, 0x8000
    stw r0, 0x194(r1)
    lfd f0, 0x190(r1)
    fsubs f0, f0, f5
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x198(r1)
    lwz r0, 0x19c(r1)
    sth r0, 0x52(r3)
    b @L_8007EF64
@L_8007EF40:
    lwz r3, 0x15c(r18)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x15c(r18)
    li r0, -0x1
    stw r0, 0x64(r3)
    lha r0, 0x100(r18)
    lwz r3, 0x15c(r18)
    sth r0, 0x52(r3)
@L_8007EF64:
    mr r14, r18
    li r17, 0x0
    addi r15, r2, -30064
    addi r16, r2, -30056
@L_8007EF74:
    lwz r3, 0x1ac(r1)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007F224
    addi r0, r17, 0xb4
    lbzx r4, r18, r0
    bl fn_80082FE4
    lbz r0, 0x71(r3)
    cmplwi r0, 0x0
    beq @L_8007F224
    lwz r3, 0x150(r14)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x0(r15)
    lis r7, 0x4330
    lwz r0, 0x0(r16)
    li r4, 0x1
    stw r3, 0x5c(r1)
    lfd f4, -30008(r2)
    stw r0, 0x60(r1)
    lbz r9, 0x5c(r1)
    lbz r3, 0x60(r1)
    lbz r10, 0x5d(r1)
    lbz r0, 0x61(r1)
    subf r3, r9, r3
    lbz r11, 0x5e(r1)
    xoris r8, r3, 0x8000
    lbz r3, 0x62(r1)
    subf r0, r10, r0
    lbz r12, 0x5f(r1)
    xoris r6, r0, 0x8000
    lbz r0, 0x63(r1)
    subf r3, r11, r3
    xoris r5, r3, 0x8000
    stw r8, 0x19c(r1)
    subf r0, r12, r0
    lfd f3, -30000(r2)
    xoris r0, r0, 0x8000
    stw r7, 0x198(r1)
    lwz r3, 0x150(r14)
    lfd f0, 0x198(r1)
    stw r9, 0x194(r1)
    fsubs f2, f0, f4
    stw r7, 0x190(r1)
    lfd f0, 0x190(r1)
    stw r6, 0x184(r1)
    fsubs f1, f0, f3
    stw r7, 0x180(r1)
    lfd f0, 0x180(r1)
    fmadds f2, f26, f2, f1
    stw r10, 0x17c(r1)
    fsubs f1, f0, f4
    stw r7, 0x178(r1)
    fctiwz f2, f2
    lfd f0, 0x178(r1)
    stfd f2, 0x188(r1)
    fsubs f0, f0, f3
    lwz r6, 0x18c(r1)
    stw r5, 0x16c(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0x168(r1)
    lfd f0, 0x168(r1)
    fctiwz f2, f1
    stw r11, 0x164(r1)
    fsubs f1, f0, f4
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    stfd f2, 0x170(r1)
    fsubs f0, f0, f3
    lwz r5, 0x174(r1)
    stw r0, 0x154(r1)
    fmadds f1, f26, f1, f0
    stw r7, 0x150(r1)
    lfd f0, 0x150(r1)
    fctiwz f2, f1
    stw r12, 0x14c(r1)
    fsubs f1, f0, f4
    stw r7, 0x148(r1)
    lfd f0, 0x148(r1)
    stfd f2, 0x158(r1)
    fsubs f0, f0, f3
    lwz r0, 0x15c(r1)
    stb r6, 0x64(r1)
    fmadds f0, f26, f1, f0
    stb r5, 0x65(r1)
    fctiwz f0, f0
    stb r0, 0x66(r1)
    stfd f0, 0x140(r1)
    lwz r0, 0x144(r1)
    stb r0, 0x67(r1)
    lwz r0, 0x64(r1)
    stw r0, 0x64(r3)
    lwz r3, 0x148(r14)
    bl fn_80109220
    lwz r3, 0x0(r15)
    lis r6, 0x4330
    lwz r0, 0x0(r16)
    stw r3, 0x50(r1)
    lfd f4, -30008(r2)
    stw r0, 0x54(r1)
    lbz r8, 0x50(r1)
    lbz r3, 0x54(r1)
    lbz r9, 0x51(r1)
    lbz r0, 0x55(r1)
    subf r3, r8, r3
    lbz r10, 0x52(r1)
    xoris r7, r3, 0x8000
    lbz r3, 0x56(r1)
    subf r0, r9, r0
    lbz r11, 0x53(r1)
    xoris r5, r0, 0x8000
    lbz r0, 0x57(r1)
    subf r3, r10, r3
    xoris r4, r3, 0x8000
    stw r7, 0x13c(r1)
    subf r0, r11, r0
    lfd f3, -30000(r2)
    xoris r0, r0, 0x8000
    stw r6, 0x138(r1)
    lwz r3, 0x148(r14)
    lfd f0, 0x138(r1)
    stw r8, 0x134(r1)
    fsubs f2, f0, f4
    stw r6, 0x130(r1)
    lfd f0, 0x130(r1)
    stw r5, 0x124(r1)
    fsubs f1, f0, f3
    stw r6, 0x120(r1)
    lfd f0, 0x120(r1)
    fmadds f2, f26, f2, f1
    stw r9, 0x11c(r1)
    fsubs f1, f0, f4
    stw r6, 0x118(r1)
    fctiwz f2, f2
    lfd f0, 0x118(r1)
    stfd f2, 0x128(r1)
    fsubs f0, f0, f3
    lwz r5, 0x12c(r1)
    stw r4, 0x10c(r1)
    fmadds f1, f26, f1, f0
    stw r6, 0x108(r1)
    lfd f0, 0x108(r1)
    fctiwz f2, f1
    stw r10, 0x104(r1)
    fsubs f1, f0, f4
    stw r6, 0x100(r1)
    lfd f0, 0x100(r1)
    stfd f2, 0x110(r1)
    fsubs f0, f0, f3
    lwz r4, 0x114(r1)
    stw r0, 0xf4(r1)
    fmadds f1, f26, f1, f0
    stw r6, 0xf0(r1)
    lfd f0, 0xf0(r1)
    fctiwz f2, f1
    stw r11, 0xec(r1)
    fsubs f1, f0, f4
    stw r6, 0xe8(r1)
    lfd f0, 0xe8(r1)
    stfd f2, 0xf8(r1)
    fsubs f0, f0, f3
    lwz r0, 0xfc(r1)
    stb r5, 0x58(r1)
    fmadds f0, f26, f1, f0
    stb r4, 0x59(r1)
    fctiwz f0, f0
    stb r0, 0x5a(r1)
    stfd f0, 0xe0(r1)
    lwz r0, 0xe4(r1)
    stb r0, 0x5b(r1)
    lwz r0, 0x58(r1)
    stw r0, 0x64(r3)
@L_8007F224:
    lwz r3, 0x1ac(r1)
    addi r14, r14, 0x4
    addi r15, r15, 0x4
    addi r16, r16, 0x4
    addi r3, r3, 0x4
    addi r17, r17, 0x1
    stw r3, 0x1ac(r1)
    cmpwi r17, 0x2
    blt @L_8007EF74
    lwz r0, 0xa4(r18)
    cmpwi r0, 0x0
    ble @L_8007F26C
    lwz r3, 0x160(r18)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x164(r18)
    li r4, 0x1
    bl fn_80109220
@L_8007F26C:
    lwz r4, 0xa4(r18)
    cmpwi r4, 0x0
    blt @L_8007F2A0
    lwz r3, 0xac(r18)
    subi r0, r3, 0x1
    cmpw r4, r0
    bge @L_8007F2A0
    lwz r3, 0x168(r18)
    li r4, 0x1
    bl fn_80109220
    lwz r3, 0x16c(r18)
    li r4, 0x1
    bl fn_80109220
@L_8007F2A0:
    lbz r0, 0xb6(r18)
    cmplwi r0, 0x0
    bne @L_8007FCC0
    lwz r3, 0xc4(r18)
    li r15, -0x1
    lwz r14, 0xc0(r18)
    addi r0, r3, 0x1
    stw r0, 0xc4(r18)
    lwz r0, 0xbc(r18)
    cmplwi r0, 0x9
    bgt @L_8007FCC0
    lis r3, jumptable_802EE868@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802EE868@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, -0x100
    li r0, -0x1
    stw r3, 0x44(r1)
    lis r7, 0x4330
    lwz r3, 0xc4(r18)
    stw r0, 0x48(r1)
    xoris r8, r3, 0x8000
    lbz r6, 0x44(r1)
    lbz r3, 0x48(r1)
    lbz r9, 0x45(r1)
    lbz r0, 0x49(r1)
    subf r3, r6, r3
    lbz r10, 0x46(r1)
    xoris r5, r3, 0x8000
    lbz r3, 0x4a(r1)
    subf r0, r9, r0
    lbz r11, 0x47(r1)
    xoris r4, r0, 0x8000
    lbz r0, 0x4b(r1)
    subf r3, r10, r3
    xoris r3, r3, 0x8000
    stw r8, 0x19c(r1)
    subf r0, r11, r0
    lfd f4, -30008(r2)
    stw r7, 0x198(r1)
    xoris r0, r0, 0x8000
    lfs f1, -30036(r2)
    lfd f0, 0x198(r1)
    stw r5, 0x194(r1)
    fsubs f2, f0, f4
    lfd f3, -30000(r2)
    stw r7, 0x190(r1)
    lfd f0, 0x190(r1)
    fmuls f5, f2, f1
    stw r6, 0x18c(r1)
    fsubs f2, f0, f4
    stw r7, 0x188(r1)
    lfd f0, 0x188(r1)
    stw r4, 0x17c(r1)
    fsubs f1, f0, f3
    stw r7, 0x178(r1)
    lfd f0, 0x178(r1)
    fmadds f2, f5, f2, f1
    stw r9, 0x174(r1)
    fsubs f1, f0, f4
    stw r7, 0x170(r1)
    fctiwz f2, f2
    lfd f0, 0x170(r1)
    stfd f2, 0x180(r1)
    fsubs f0, f0, f3
    lwz r4, 0x184(r1)
    stw r3, 0x164(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    fctiwz f2, f1
    stw r10, 0x15c(r1)
    fsubs f1, f0, f4
    stw r7, 0x158(r1)
    lfd f0, 0x158(r1)
    stfd f2, 0x168(r1)
    fsubs f0, f0, f3
    lwz r3, 0x16c(r1)
    stw r0, 0x14c(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x148(r1)
    lfd f0, 0x148(r1)
    fctiwz f2, f1
    stw r11, 0x144(r1)
    fsubs f1, f0, f4
    stw r7, 0x140(r1)
    lfd f0, 0x140(r1)
    stfd f2, 0x150(r1)
    fsubs f0, f0, f3
    lwz r0, 0x154(r1)
    stb r4, 0x4c(r1)
    fmadds f0, f5, f1, f0
    stb r3, 0x4d(r1)
    fctiwz f0, f0
    stb r0, 0x4e(r1)
    stfd f0, 0x138(r1)
    lwz r0, 0x13c(r1)
    stb r0, 0x4f(r1)
    rlwinm r14, r14, 2, 0, 29
    lwz r0, 0x4c(r1)
    add r5, r18, r14
    li r4, 0x0
    lwz r3, 0x200(r5)
    stw r0, 0x64(r3)
    lwz r3, 0x440(r5)
    bl fn_80109220
    add r3, r18, r14
    li r4, 0x0
    lwz r3, 0x320(r3)
    bl fn_80109220
    lwz r0, 0xc4(r18)
    cmpwi r0, 0x10
    bne @L_8007FCC0
    li r3, 0x0
    li r0, 0x2
    stw r3, 0xc4(r18)
    stw r0, 0xbc(r18)
    b @L_8007FCC0
    li r15, 0x1
    rlwinm r16, r14, 2, 0, 29
    li r4, 0x0
    add r3, r18, r16
    lwz r3, 0x440(r3)
    bl fn_80109220
    add r3, r18, r16
    li r4, 0x0
    lwz r3, 0x320(r3)
    bl fn_80109220
    b @L_8007F4B0
    li r15, 0x1
@L_8007F4B0:
    lwz r3, 0x4d0(r18)
    li r4, 0x1
    bl fn_80109220
    rlwinm r0, r14, 2, 0, 29
    lwz r3, 0x4d0(r18)
    add r5, r18, r0
    cmpwi r15, 0x1
    lwz r4, 0x200(r5)
    lha r0, 0x50(r4)
    sth r0, 0x50(r3)
    lwz r4, 0x200(r5)
    lwz r3, 0x4d0(r18)
    lha r0, 0x52(r4)
    sth r0, 0x52(r3)
    bne @L_8007F648
    li r3, -0x100
    li r0, -0x89
    stw r3, 0x38(r1)
    lis r7, 0x4330
    lwz r3, 0xc4(r18)
    stw r0, 0x3c(r1)
    xoris r8, r3, 0x8000
    lbz r6, 0x38(r1)
    lbz r3, 0x3c(r1)
    lbz r9, 0x39(r1)
    lbz r0, 0x3d(r1)
    subf r3, r6, r3
    lbz r10, 0x3a(r1)
    xoris r5, r3, 0x8000
    lbz r3, 0x3e(r1)
    subf r0, r9, r0
    lbz r11, 0x3b(r1)
    xoris r4, r0, 0x8000
    lbz r0, 0x3f(r1)
    subf r3, r10, r3
    xoris r3, r3, 0x8000
    stw r8, 0x19c(r1)
    subf r0, r11, r0
    lfd f4, -30008(r2)
    stw r7, 0x198(r1)
    xoris r0, r0, 0x8000
    lfs f1, -30036(r2)
    lfd f0, 0x198(r1)
    stw r5, 0x194(r1)
    fsubs f2, f0, f4
    lfd f3, -30000(r2)
    stw r7, 0x190(r1)
    lfd f0, 0x190(r1)
    fmuls f5, f2, f1
    stw r6, 0x18c(r1)
    fsubs f2, f0, f4
    stw r7, 0x188(r1)
    lfd f0, 0x188(r1)
    stw r4, 0x17c(r1)
    fsubs f1, f0, f3
    stw r7, 0x178(r1)
    lfd f0, 0x178(r1)
    fmadds f2, f5, f2, f1
    stw r9, 0x174(r1)
    fsubs f1, f0, f4
    stw r7, 0x170(r1)
    fctiwz f2, f2
    lfd f0, 0x170(r1)
    stfd f2, 0x180(r1)
    fsubs f0, f0, f3
    lwz r4, 0x184(r1)
    stw r3, 0x164(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    fctiwz f2, f1
    stw r10, 0x15c(r1)
    fsubs f1, f0, f4
    stw r7, 0x158(r1)
    lfd f0, 0x158(r1)
    stfd f2, 0x168(r1)
    fsubs f0, f0, f3
    lwz r3, 0x16c(r1)
    stw r0, 0x14c(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x148(r1)
    lfd f0, 0x148(r1)
    fctiwz f2, f1
    stw r11, 0x144(r1)
    fsubs f1, f0, f4
    stw r7, 0x140(r1)
    lfd f0, 0x140(r1)
    stfd f2, 0x150(r1)
    fsubs f0, f0, f3
    lwz r0, 0x154(r1)
    stb r4, 0x40(r1)
    fmadds f0, f5, f1, f0
    stb r3, 0x41(r1)
    fctiwz f0, f0
    stb r0, 0x42(r1)
    stfd f0, 0x138(r1)
    lwz r0, 0x13c(r1)
    stb r0, 0x43(r1)
    lwz r0, 0x40(r1)
    lwz r3, 0x4d0(r18)
    stw r0, 0x64(r3)
    b @L_8007F7A0
@L_8007F648:
    li r3, -0x89
    li r0, -0x100
    stw r3, 0x2c(r1)
    lis r7, 0x4330
    lwz r3, 0xc4(r18)
    stw r0, 0x30(r1)
    xoris r8, r3, 0x8000
    lbz r6, 0x2c(r1)
    lbz r3, 0x30(r1)
    lbz r9, 0x2d(r1)
    lbz r0, 0x31(r1)
    subf r3, r6, r3
    lbz r10, 0x2e(r1)
    xoris r5, r3, 0x8000
    lbz r3, 0x32(r1)
    subf r0, r9, r0
    lbz r11, 0x2f(r1)
    xoris r4, r0, 0x8000
    lbz r0, 0x33(r1)
    subf r3, r10, r3
    xoris r3, r3, 0x8000
    stw r8, 0x19c(r1)
    subf r0, r11, r0
    lfd f4, -30008(r2)
    stw r7, 0x198(r1)
    xoris r0, r0, 0x8000
    lfs f1, -30036(r2)
    lfd f0, 0x198(r1)
    stw r5, 0x194(r1)
    fsubs f2, f0, f4
    lfd f3, -30000(r2)
    stw r7, 0x190(r1)
    lfd f0, 0x190(r1)
    fmuls f5, f2, f1
    stw r6, 0x18c(r1)
    fsubs f2, f0, f4
    stw r7, 0x188(r1)
    lfd f0, 0x188(r1)
    stw r4, 0x17c(r1)
    fsubs f1, f0, f3
    stw r7, 0x178(r1)
    lfd f0, 0x178(r1)
    fmadds f2, f5, f2, f1
    stw r9, 0x174(r1)
    fsubs f1, f0, f4
    stw r7, 0x170(r1)
    fctiwz f2, f2
    lfd f0, 0x170(r1)
    stfd f2, 0x180(r1)
    fsubs f0, f0, f3
    lwz r4, 0x184(r1)
    stw r3, 0x164(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    fctiwz f2, f1
    stw r10, 0x15c(r1)
    fsubs f1, f0, f4
    stw r7, 0x158(r1)
    lfd f0, 0x158(r1)
    stfd f2, 0x168(r1)
    fsubs f0, f0, f3
    lwz r3, 0x16c(r1)
    stw r0, 0x14c(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x148(r1)
    lfd f0, 0x148(r1)
    fctiwz f2, f1
    stw r11, 0x144(r1)
    fsubs f1, f0, f4
    stw r7, 0x140(r1)
    lfd f0, 0x140(r1)
    stfd f2, 0x150(r1)
    fsubs f0, f0, f3
    lwz r0, 0x154(r1)
    stb r4, 0x34(r1)
    fmadds f0, f5, f1, f0
    stb r3, 0x35(r1)
    fctiwz f0, f0
    stb r0, 0x36(r1)
    stfd f0, 0x138(r1)
    lwz r0, 0x13c(r1)
    stb r0, 0x37(r1)
    lwz r0, 0x34(r1)
    lwz r3, 0x4d0(r18)
    stw r0, 0x64(r3)
@L_8007F7A0:
    lwz r0, 0xc4(r18)
    cmpwi r0, 0x10
    bne @L_8007FCC0
    li r0, 0x0
    stw r0, 0xc4(r18)
    lwz r0, 0xbc(r18)
    add r0, r0, r15
    stw r0, 0xbc(r18)
    b @L_8007FCC0
    lis r3, 0x2aab
    li r17, 0x0
    subi r0, r3, 0x5555
    mulhw r3, r0, r14
    rlwinm r0, r3, 1, 31, 31
    add r0, r3, r0
    extsb r16, r0
    mulli r0, r16, 0x6
    lwz r14, 0xa8(r1)
    rlwinm r19, r0, 2, 0, 29
    b @L_8007FAC4
@L_8007F7F0:
    lbz r4, 0xb5(r18)
    mr r3, r14
    mr r5, r16
    mr r6, r17
    bl fn_80082EA4
    lbz r0, 0xc(r3)
    cmplwi r0, 0x0
    bne @L_8007FABC
    addi r15, r19, 0x200
    li r4, 0x1
    lwzx r3, r18, r15
    bl fn_80109220
    lwz r3, 0xc4(r18)
    lis r0, 0x4330
    stw r0, 0x198(r1)
    xoris r3, r3, 0x8000
    lfd f0, -30008(r2)
    stw r3, 0x19c(r1)
    lfs f2, -30016(r2)
    lfd f3, 0x198(r1)
    lfs f1, -30040(r2)
    fsubs f3, f3, f0
    fmuls f5, f3, f2
    fcmpo cr0, f5, f1
    bge @L_8007F988
    li r4, -0x100
    li r3, -0x89
    stw r4, 0x20(r1)
    lfd f4, -30000(r2)
    stw r3, 0x24(r1)
    lbz r8, 0x20(r1)
    lbz r4, 0x24(r1)
    lbz r9, 0x21(r1)
    lbz r3, 0x25(r1)
    subf r4, r8, r4
    lbz r10, 0x22(r1)
    xoris r7, r4, 0x8000
    lbz r4, 0x26(r1)
    subf r3, r9, r3
    lbz r11, 0x23(r1)
    xoris r6, r3, 0x8000
    lbz r3, 0x27(r1)
    subf r4, r10, r4
    xoris r5, r4, 0x8000
    stw r7, 0x19c(r1)
    subf r4, r11, r3
    lwzx r3, r18, r15
    xoris r4, r4, 0x8000
    stw r0, 0x198(r1)
    lfd f1, 0x198(r1)
    stw r8, 0x194(r1)
    fsubs f3, f1, f0
    stw r0, 0x190(r1)
    lfd f1, 0x190(r1)
    stw r6, 0x184(r1)
    fsubs f2, f1, f4
    stw r0, 0x180(r1)
    lfd f1, 0x180(r1)
    fmadds f3, f5, f3, f2
    stw r9, 0x17c(r1)
    fsubs f2, f1, f0
    stw r0, 0x178(r1)
    fctiwz f3, f3
    lfd f1, 0x178(r1)
    stfd f3, 0x188(r1)
    fsubs f1, f1, f4
    lwz r6, 0x18c(r1)
    stw r5, 0x16c(r1)
    fmadds f2, f5, f2, f1
    stw r0, 0x168(r1)
    lfd f1, 0x168(r1)
    fctiwz f3, f2
    stw r10, 0x164(r1)
    fsubs f2, f1, f0
    stw r0, 0x160(r1)
    lfd f1, 0x160(r1)
    stfd f3, 0x170(r1)
    fsubs f1, f1, f4
    lwz r5, 0x174(r1)
    stw r4, 0x154(r1)
    fmadds f2, f5, f2, f1
    stw r0, 0x150(r1)
    lfd f1, 0x150(r1)
    fctiwz f2, f2
    stw r11, 0x14c(r1)
    fsubs f1, f1, f0
    stw r0, 0x148(r1)
    lfd f0, 0x148(r1)
    stfd f2, 0x158(r1)
    fsubs f0, f0, f4
    lwz r0, 0x15c(r1)
    stb r6, 0x28(r1)
    fmadds f0, f5, f1, f0
    stb r5, 0x29(r1)
    fctiwz f0, f0
    stb r0, 0x2a(r1)
    stfd f0, 0x140(r1)
    lwz r0, 0x144(r1)
    stb r0, 0x2b(r1)
    lwz r0, 0x28(r1)
    stw r0, 0x64(r3)
    b @L_8007FABC
@L_8007F988:
    li r4, -0x89
    li r3, -0x100
    stw r4, 0x14(r1)
    fsubs f5, f5, f1
    lfd f4, -30000(r2)
    stw r3, 0x18(r1)
    lbz r7, 0x14(r1)
    lbz r4, 0x18(r1)
    lbz r8, 0x15(r1)
    lbz r3, 0x19(r1)
    subf r4, r7, r4
    lbz r9, 0x16(r1)
    xoris r6, r4, 0x8000
    lbz r4, 0x1a(r1)
    subf r3, r8, r3
    lbz r10, 0x17(r1)
    xoris r5, r3, 0x8000
    lbz r3, 0x1b(r1)
    subf r4, r9, r4
    xoris r4, r4, 0x8000
    stw r6, 0x19c(r1)
    subf r3, r10, r3
    xoris r3, r3, 0x8000
    stw r0, 0x198(r1)
    lfd f1, 0x198(r1)
    stw r7, 0x194(r1)
    fsubs f3, f1, f0
    stw r0, 0x190(r1)
    lfd f1, 0x190(r1)
    stw r5, 0x184(r1)
    fsubs f2, f1, f4
    stw r0, 0x180(r1)
    lfd f1, 0x180(r1)
    fmadds f3, f5, f3, f2
    stw r8, 0x17c(r1)
    fsubs f2, f1, f0
    stw r0, 0x178(r1)
    fctiwz f3, f3
    lfd f1, 0x178(r1)
    stfd f3, 0x188(r1)
    fsubs f1, f1, f4
    lwz r5, 0x18c(r1)
    stw r4, 0x16c(r1)
    fmadds f2, f5, f2, f1
    stw r0, 0x168(r1)
    lfd f1, 0x168(r1)
    fctiwz f3, f2
    stw r9, 0x164(r1)
    fsubs f2, f1, f0
    stw r0, 0x160(r1)
    lfd f1, 0x160(r1)
    stfd f3, 0x170(r1)
    fsubs f1, f1, f4
    lwz r4, 0x174(r1)
    stw r3, 0x154(r1)
    fmadds f2, f5, f2, f1
    stw r0, 0x150(r1)
    lfd f1, 0x150(r1)
    fctiwz f2, f2
    stw r10, 0x14c(r1)
    fsubs f1, f1, f0
    stw r0, 0x148(r1)
    lfd f0, 0x148(r1)
    stfd f2, 0x158(r1)
    fsubs f0, f0, f4
    lwz r0, 0x15c(r1)
    stb r5, 0x1c(r1)
    fmadds f0, f5, f1, f0
    stb r4, 0x1d(r1)
    fctiwz f0, f0
    stb r0, 0x1e(r1)
    stfd f0, 0x140(r1)
    lwz r0, 0x144(r1)
    stb r0, 0x1f(r1)
    lwz r0, 0x1c(r1)
    lwzx r3, r18, r15
    stw r0, 0x64(r3)
@L_8007FABC:
    addi r19, r19, 0x4
    addi r17, r17, 0x1
@L_8007FAC4:
    lbz r0, 0x1d(r14)
    extsb r3, r17
    extsb r0, r0
    cmpw r3, r0
    blt @L_8007F7F0
    lwz r0, 0xc4(r18)
    cmpwi r0, 0x40
    bne @L_8007FCC0
    li r0, 0x0
    stw r0, 0xc4(r18)
    b @L_8007FCC0
    rlwinm r15, r14, 2, 0, 29
    li r4, 0x0
    add r3, r18, r15
    lwz r3, 0x440(r3)
    bl fn_80109220
    add r3, r18, r15
    li r4, 0x0
    lwz r3, 0x320(r3)
    bl fn_80109220
    li r3, -0x1
    li r0, -0x100
    stw r3, 0x8(r1)
    lis r7, 0x4330
    lwz r3, 0xc4(r18)
    stw r0, 0xc(r1)
    xoris r8, r3, 0x8000
    lbz r6, 0x8(r1)
    lbz r3, 0xc(r1)
    lbz r9, 0x9(r1)
    lbz r0, 0xd(r1)
    subf r3, r6, r3
    lbz r10, 0xa(r1)
    xoris r5, r3, 0x8000
    lbz r3, 0xe(r1)
    subf r0, r9, r0
    lbz r11, 0xb(r1)
    xoris r4, r0, 0x8000
    lbz r0, 0xf(r1)
    subf r3, r10, r3
    xoris r3, r3, 0x8000
    stw r8, 0x19c(r1)
    subf r0, r11, r0
    lfd f4, -30008(r2)
    stw r7, 0x198(r1)
    xoris r0, r0, 0x8000
    lfs f1, -30036(r2)
    lfd f0, 0x198(r1)
    stw r5, 0x194(r1)
    fsubs f2, f0, f4
    lfd f3, -30000(r2)
    stw r7, 0x190(r1)
    lfd f0, 0x190(r1)
    fmuls f5, f2, f1
    stw r6, 0x18c(r1)
    fsubs f2, f0, f4
    stw r7, 0x188(r1)
    lfd f0, 0x188(r1)
    stw r4, 0x17c(r1)
    fsubs f1, f0, f3
    stw r7, 0x178(r1)
    lfd f0, 0x178(r1)
    fmadds f2, f5, f2, f1
    stw r9, 0x174(r1)
    fsubs f1, f0, f4
    stw r7, 0x170(r1)
    fctiwz f2, f2
    lfd f0, 0x170(r1)
    stfd f2, 0x180(r1)
    fsubs f0, f0, f3
    lwz r4, 0x184(r1)
    stw r3, 0x164(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x160(r1)
    lfd f0, 0x160(r1)
    fctiwz f2, f1
    stw r10, 0x15c(r1)
    fsubs f1, f0, f4
    stw r7, 0x158(r1)
    lfd f0, 0x158(r1)
    stfd f2, 0x168(r1)
    fsubs f0, f0, f3
    lwz r3, 0x16c(r1)
    stw r0, 0x14c(r1)
    fmadds f1, f5, f1, f0
    stw r7, 0x148(r1)
    lfd f0, 0x148(r1)
    fctiwz f2, f1
    stw r11, 0x144(r1)
    fsubs f1, f0, f4
    stw r7, 0x140(r1)
    lfd f0, 0x140(r1)
    stfd f2, 0x150(r1)
    fsubs f0, f0, f3
    lwz r0, 0x154(r1)
    stb r4, 0x10(r1)
    fmadds f0, f5, f1, f0
    stb r3, 0x11(r1)
    fctiwz f0, f0
    stb r0, 0x12(r1)
    stfd f0, 0x138(r1)
    lwz r0, 0x13c(r1)
    stb r0, 0x13(r1)
    rlwinm r0, r14, 2, 0, 29
    lwz r4, 0x10(r1)
    add r3, r18, r0
    lwz r3, 0x200(r3)
    stw r4, 0x64(r3)
    lwz r0, 0xc4(r18)
    cmpwi r0, 0x10
    bne @L_8007FCC0
    li r0, 0x9
    stw r0, 0xbc(r18)
    b @L_8007FCC0
    rlwinm r14, r14, 2, 0, 29
    li r4, 0x0
    add r3, r18, r14
    lwz r3, 0x440(r3)
    bl fn_80109220
    add r3, r18, r14
    li r4, 0x0
    lwz r3, 0x320(r3)
    bl fn_80109220
    add r3, r18, r14
    li r4, 0x0
    lwz r3, 0x200(r3)
    bl fn_80109220
@L_8007FCC0:
    li r3, 0xe8
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8007FD20
    lwz r5, 0x1c(r3)
    li r3, -0x56
    li r0, -0x1
    b @L_8007FD18
@L_8007FCE0:
    lbz r4, 0x5(r5)
    rlwinm r4, r4, 0, 31, 31
    cmpwi r4, 0x0
    beq @L_8007FD14
    lbz r4, 0xb6(r18)
    cmplwi r4, 0x0
    bne @L_8007FD10
    lwz r4, 0xbc(r18)
    cmpwi r4, 0x0
    beq @L_8007FD10
    stw r3, 0x64(r5)
    b @L_8007FD14
@L_8007FD10:
    stw r0, 0x64(r5)
@L_8007FD14:
    lwz r5, 0x0(r5)
@L_8007FD18:
    cmplwi r5, 0x0
    bne @L_8007FCE0
@L_8007FD20:
    psq_l f31, 0x258(r1), 0, 0
    lfd f31, 0x250(r1)
    psq_l f30, 0x248(r1), 0, 0
    lfd f30, 0x240(r1)
    psq_l f29, 0x238(r1), 0, 0
    lfd f29, 0x230(r1)
    psq_l f28, 0x228(r1), 0, 0
    lfd f28, 0x220(r1)
    psq_l f27, 0x218(r1), 0, 0
    lfd f27, 0x210(r1)
    psq_l f26, 0x208(r1), 0, 0
    lfd f26, 0x200(r1)
    lmw r14, 0x1b8(r1)
    lwz r0, 0x264(r1)
    mtlr r0
    addi r1, r1, 0x260
    blr

