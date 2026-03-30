    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    mr r29, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    cmplwi r31, 0x0
    rlwinm r30, r3, 0, 16, 31
    bne @L_80205C64
    li r3, 0x0
    b @L_80206294
@L_80205C64:
    bne @L_80205C70
    li r0, 0x0
    b @L_80205F50
@L_80205C70:
    bne @L_80205C7C
    li r0, 0x0
    b @L_80205D98
@L_80205C7C:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205C94
    li r0, 0x0
    b @L_80205D98
@L_80205C94:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    bne @L_80205CB8
    li r0, 0x0
    b @L_80205D98
@L_80205CB8:
    bne @L_80205CC4
    li r0, 0x0
    b @L_80205D84
@L_80205CC4:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205CDC
    li r0, 0x0
    b @L_80205D84
@L_80205CDC:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205D00
    li r0, 0x0
    b @L_80205D84
@L_80205D00:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205D14
    li r0, 0x0
    b @L_80205D84
@L_80205D14:
    cmplwi r28, 0x0
    bne @L_80205D24
    li r3, 0x0
    b @L_80205D38
@L_80205D24:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80205D38:
    cmplwi r3, 0x0
    bne @L_80205D48
    li r0, 0x0
    b @L_80205D84
@L_80205D48:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205D5C
    li r0, 0x0
    b @L_80205D84
@L_80205D5C:
    mr r3, r28
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80205D80
    li r0, 0x0
    b @L_80205D84
@L_80205D80:
    li r0, 0x1
@L_80205D84:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205D94
    li r0, 0x0
    b @L_80205D98
@L_80205D94:
    li r0, 0x1
@L_80205D98:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205DA8
    li r0, 0x0
    b @L_80205F50
@L_80205DA8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80205DCC
    li r0, 0x0
    b @L_80205F50
@L_80205DCC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    bne @L_80205DF0
    li r0, 0x0
    b @L_80205F3C
@L_80205DF0:
    bne @L_80205DFC
    li r0, 0x0
    b @L_80205EBC
@L_80205DFC:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205E14
    li r0, 0x0
    b @L_80205EBC
@L_80205E14:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205E38
    li r0, 0x0
    b @L_80205EBC
@L_80205E38:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205E4C
    li r0, 0x0
    b @L_80205EBC
@L_80205E4C:
    cmplwi r28, 0x0
    bne @L_80205E5C
    li r3, 0x0
    b @L_80205E70
@L_80205E5C:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80205E70:
    cmplwi r3, 0x0
    bne @L_80205E80
    li r0, 0x0
    b @L_80205EBC
@L_80205E80:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205E94
    li r0, 0x0
    b @L_80205EBC
@L_80205E94:
    mr r3, r28
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80205EB8
    li r0, 0x0
    b @L_80205EBC
@L_80205EB8:
    li r0, 0x1
@L_80205EBC:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205ECC
    li r0, 0x0
    b @L_80205F3C
@L_80205ECC:
    mr r3, r28
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80205EF0
    li r0, 0x0
    b @L_80205F3C
@L_80205EF0:
    cmplwi r28, 0x0
    bne @L_80205F00
    li r3, 0x0
    b @L_80205F14
@L_80205F00:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80205F14:
    cmplwi r3, 0x0
    bne @L_80205F24
    li r0, 0x0
    b @L_80205F3C
@L_80205F24:
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205F38
    li r0, 0x0
    b @L_80205F3C
@L_80205F38:
    li r0, 0x1
@L_80205F3C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205F4C
    li r0, 0x0
    b @L_80205F50
@L_80205F4C:
    li r0, 0x1
@L_80205F50:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205F60
    li r3, 0x0
    b @L_80206294
@L_80205F60:
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80205F9C
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_80205F9C
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80206038
@L_80205F9C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80205FDC
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_8020600C
@L_80205FDC:
    cmplwi r28, 0x0
    bne @L_80205FEC
    li r3, 0x0
    b @L_80206000
@L_80205FEC:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80206000:
    li r4, 0x12
    bl fn_80121ADC
    b @L_80206060
@L_8020600C:
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80206028
    li r3, 0x0
    b @L_80206060
@L_80206028:
    mr r3, r28
    li r4, 0x12
    bl fn_8011B67C
    b @L_80206060
@L_80206038:
    li r3, 0x12
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_80206054
    li r3, 0x0
    b @L_80206060
@L_80206054:
    mr r3, r31
    li r4, 0x12
    bl fn_8011B67C
@L_80206060:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80206178
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802060A8
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_802060A8
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80206144
@L_802060A8:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802060E8
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80206118
@L_802060E8:
    cmplwi r28, 0x0
    bne @L_802060F8
    li r3, 0x0
    b @L_8020610C
@L_802060F8:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020610C:
    li r4, 0x22
    bl fn_80121ADC
    b @L_8020616C
@L_80206118:
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80206134
    li r3, 0x0
    b @L_8020616C
@L_80206134:
    mr r3, r28
    li r4, 0x22
    bl fn_8011B67C
    b @L_8020616C
@L_80206144:
    li r3, 0x22
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_80206160
    li r3, 0x0
    b @L_8020616C
@L_80206160:
    mr r3, r31
    li r4, 0x22
    bl fn_8011B67C
@L_8020616C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80206290
@L_80206178:
    rlwinm. r0, r29, 0, 24, 31
    beq @L_80206288
    mr r3, r31
    li r4, 0x0
    li r5, 0xf8
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    bl fn_80209CB4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80206288
    mr r3, r28
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    lis r4, -32736
    extsb r27, r3
    addi r6, r4, 25256
    mr r3, r31
    mr r4, r29
    mr r5, r30
    li r7, 0x1
    li r8, 0x0
    li r9, -0x1
    bl fn_8022B2CC
    mr r4, r30
    bl fn_801F0134
    mr r0, r3
    mr r3, r31
    mr r28, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80206288
    mr r4, r27
    mr r5, r29
    mr r6, r28
    li r7, 0x1
    bl fn_802099AC
    mr r3, r31
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_80206288
    lis r4, -32713
    mr r5, r31
    addi r8, r4, 23720
    li r6, 0x13
    li r4, 0x0
    li r7, 0x0
    bl fn_801F11CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80206288
    mr r3, r28
    mr r4, r29
    bl fn_8020D878
@L_80206288:
    li r3, 0x0
    b @L_80206294
@L_80206290:
    li r3, 0x1
@L_80206294:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
